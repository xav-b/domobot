#!/usr/bin/python
# -*- coding: utf-8 -*-


import psutil
import time
import sys


class ProcessInfo():
    """ Cette classe récupère les informations sur un processus
    utilisation CPU, consommation mémoire, etc
    """
    def __init__(self, name, interval, callback):
        """ Initialisation de l’objet
        @param pid : identifiant du processus
        @param interval : intervalle entre la lecture des valeurs
        @param callback : fonction de rappel
        """
        # initialisation des variables
        self.pid = None
        self._callback = callback
        self._interval = interval
        found = 0
        listProcess = psutil.process_iter()
        for process in listProcess:
            if ( process.name == name):
                self.name = name
                self.p = psutil.Process( process.pid )
                found = 1
                self.pid = self.p.pid
        if not found:
            err = "** No process named %s found, exiting..." % (name)
            sys.exit(err)
        print '[INIT] Process pid found (' + str(self.p.status) + '), beginning analysis.'
        print '[Init] Run by', self.p.username,'from', self.p.getcwd, '(', self.p.create_time, ')'

    def start(self):
        """ Récupération des informations sur le process tant que le
        processus est en vie
        """
        if self.pid == None:
            return
        process_loop = True
        while process_loop == True:
            # On vérifie si le processus existe toujours
            if not psutil.pid_exists(self.pid):
                print("Le processus ‘%s’ n’existe plus : on arrête sa surveillance" % self.pid)
                process_loop = False
                continue
                # On récupère les valeurs et on les donne à la fonction de rappel
            values = self._get_values()
            self._callback(self.pid, values)
            time.sleep(self._interval)

    def _get_values(self):
        """ Récupération des valeurs intéressantes et stockage dans un
        dictionnaire
        """
        cpu_percent = round(self.p.get_cpu_percent(), 1)
        # Récupération des informations mémoire en conversion en Mo
        memory_total_phymem = round(psutil.TOTAL_PHYMEM / (1024 *1024), 0)
        memory_info = self.p.get_memory_info()
        memory_rss = round(memory_info[0] / (1024 * 1024), 1)
        memory_vsz = round(memory_info[1] / (1024 * 1024), 1)
        memory_percent = round(self.p.get_memory_percent(),1)
        values = {"pid" : self.pid,
            "cpu_percent" : cpu_percent,
            "memory_total_phymem" : memory_total_phymem,
            "memory_rss" : memory_rss,
            "memory_vsz" : memory_vsz,
            "memory_percent" : memory_percent,
            }
        return values

'''
# fonction de rappel pour tester la classe
def display(pid, data):
    print("DATA (%s) = %s" % (pid, str(data)))

# instanciation de la classe pour test

if __name__ == "__main__":
    Kinect = ProcessInfo(sys.argv[1], 5, display)
    Kinect.start()
'''
