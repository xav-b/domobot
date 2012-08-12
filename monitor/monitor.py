#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
if os.name != 'posix':
    sys.exit('platform not supported')
import time
import curses
import atexit
from datetime import datetime, timedelta
import psutil

class ProcessMonitor:
    def __init__(self, name):
        found = 0
        iter = psutil.process_iter()
        for process in iter:
            if ( process.name == name ):
                self.process = psutil.Process( process.pid )
                found = 1
        if not found :
            sys.exit('** No process named KinectTuio found...')
        else:
            print 'Process pid found (' + str(self.process.status) + '), beginning analysis.'
            print 'Run by', self.process.username,'from', self.process.getcwd, '(', self.process.create_time, ')'

    def context(self, cpus, memory, network):
        #TODO Store the very first value as reference
        if ( cpus ):
            print 'Available CPUs:', psutil.NUM_CPUS
            print psutil.cpu_times()
            print psutil.cpu_percent(interval=1, percpu=True)
        if ( memory ):
            print 'Memory counters:', psutil.disk_io_counters()
        if ( network ):
            print 'Nework trafic:', psutil.network_io_counters() 




## -------------------------------------------------------------------------
## Curses style
def tear_down():
    win.keypad(0)
    curses.nocbreak()
    curses.echo()
    curses.endwin()

win = curses.initscr()
atexit.register(tear_down)
curses.endwin()
lineno = 0

def print_line(line, highlight=False, lastLine=False):
    global lineno
    try:
        if highlight:
            line += ' ' * (win.getmaxyx()[1] - len(line)) 
            win.addstr(lineno, 0, line, curses.A_REVERSE)
        else:
            win.addstr(lineno, 0, line, 0)
    except curses.error:
        lineno = 0
        win.refresh()
        raise
    else:
        lineno += 1
    if lastLine:
        lineno = 0
## ------------------------------------------------------------------------

def bytes2human(n):
    """
    >>> bytes2human(10000)
    '9K'
    >>> bytes2human(100001221)
    '95M'
    """
    symbols = ('K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y')
    prefix = {}
    for i, s in enumerate(symbols):
        prefix[s] = 1 << (i+1)*10
    for s in reversed(symbols):
        if n >= prefix[s]:
            value = int(float(n) / prefix[s])
            return '%s%s' % (value, s)
    return "%sB" % n

def poll(interval):
    time.sleep(interval)

    # then retrieve the same info again
    try:
        KinectTuio._username = KinectTuio.username
        KinectTuio._meminfo = KinectTuio.get_memory_info()
        KinectTuio._mempercent = KinectTuio.get_memory_percent()
        KinectTuio._cpu_percent = KinectTuio.get_cpu_percent(interval=0)
        KinectTuio._cpu_times = KinectTuio.get_cpu_times()
        KinectTuio._name = KinectTuio.name
        KinectTuio._status = str(KinectTuio.status)
    except psutil.NoSuchProcess:
        sys.exit('No longer process named KinectTuio')

    return str(KinectTuio._status)

def print_header():
    """Print system-related info, above the process list."""

    def get_dashes(perc):
        dashes =  "|" * int((float(perc) / 10 * 4))
        empty_dashes = " " * (40 - len(dashes))
        return dashes, empty_dashes

    # cpu usage
    for cpu_num, perc in enumerate(psutil.cpu_percent(interval=0, percpu=True)):
        dashes, empty_dashes = get_dashes(perc)
        print_line(" CPU%-2s [%s%s] %5s%%" % (cpu_num, dashes, empty_dashes,
                                              perc))
    # physmem usage (on linux we include buffers and cached values
    # to match htop results)
    phymem = psutil.phymem_usage()
    dashes, empty_dashes = get_dashes(phymem.percent)
    buffers = getattr(psutil, 'phymem_buffers', lambda: 0)()
    cached = getattr(psutil, 'cached_phymem', lambda: 0)()
    used = phymem.total - (phymem.free + buffers + cached)
    line = " Mem   [%s%s] %5s%% %6s/%s" % (
        dashes, empty_dashes,
        phymem.percent,
        str(int(used / 1024 / 1024)) + "M",
        str(int(phymem.total / 1024 / 1024)) + "M"
    )
    print_line(line)

    # swap usage
    vmem = psutil.virtmem_usage()
    dashes, empty_dashes = get_dashes(vmem.percent)
    line = " Swap  [%s%s] %5s%% %6s/%s" % (
        dashes, empty_dashes,
        vmem.percent,
        str(int(vmem.used / 1024 / 1024)) + "M",
        str(int(vmem.total / 1024 / 1024)) + "M"
    )
    print_line(line)

    # load average, uptime
    uptime = datetime.now() - datetime.fromtimestamp(psutil.BOOT_TIME)
    av1, av2, av3 = os.getloadavg()
    line = " Load average: %.2f %.2f %.2f  Uptime: %s" \
            % (av1, av2, av3, str(uptime).split('.')[0])
    print_line(line)

def refresh_window(KinectTuio, statue):
    """Print results on screen by using curses."""
    curses.endwin()
    templ = "%-6s %-8s %5s %5s %6s %4s %9s  %2s"
    win.erase()
    header = templ % ("PID", "USER", "VIRT", "RES", "CPU%", "MEM%",
                      "TIME+", "NAME")
    print_header()
    print_line("")
    print_line(header, highlight=True)
    # TIME+ column shows process CPU cumulative time and it
    # is expressed as: "mm:ss.ms"
    ctime = timedelta(seconds=sum(KinectTuio._cpu_times))
    ctime = "%s:%s.%s" % (ctime.seconds // 60 % 60,
                          str((ctime.seconds % 60)).zfill(2),
                          str(ctime.microseconds)[:2])
    line = templ % (KinectTuio.pid,
                    KinectTuio._username[:8],
                    bytes2human(KinectTuio._meminfo.vms),
                    bytes2human(KinectTuio._meminfo.rss),
                    KinectTuio._cpu_percent,
                    round(KinectTuio._mempercent, 1),
                    ctime,
                    KinectTuio._name,
                    )
    try:
        print_line(line, highlight=False, lastLine=True)
    except curses.error:
        print 'curses error'
    win.refresh()


#def main():
#print 'Running KinectTuio process'
#os.system('../kinect/Tracker/KinectTuio')
#print 'Running monitor'
kinect = ProcessMonitor('KinectTuio')
KinectTuio = kinect.process
try:
    interval = 1
    while 1:
        statue = poll(interval)
        refresh_window(KinectTuio, statue)
except (KeyboardInterrupt, SystemExit):
    pass

#if __name__ == '__main__':
    #main()


# Instantiate and checking if process exists ------------------------------
#kinect = ProcessMonitor('bash')
#print kinect.process
#print kinect.process.get_cpu_percent()
#print kinect.process.get_memory_percent()
#print kinect.process.get_memory_info()
# Print general running system infos
#kinect.context(True, True, True)
