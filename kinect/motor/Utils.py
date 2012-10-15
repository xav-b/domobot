#!/usr/bin/python
# -*- coding: utf8 -*-

import argparse, sys, json
from time import gmtime, strftime

class Utils:
    """Some utilies functions"""
    def debug(self, msg):
        print '[DEBUG]',  strftime("%d %b %Y %H:%M:%S +0000", gmtime()) + ": ",
        print msg

    def fatal(self, msg):
        print '[FATAL]',  strftime("%d %b %Y %H:%M:%S +0000", gmtime()) + ": ", msg
        sys.exit('** Exiting...')

    def parseArgs(self, descr, isConfig):
        """ Parsing command line arguments """
        self.debug ('Parsing command line args...')
        parser = argparse.ArgumentParser(description=descr)
        parser.add_argument('-v', '--version', action='version', version='%(prog)s v0.1 Licence rien du tout', help='Print program version')
        parser.add_argument('-f', '--file', action='store', required=isConfig, help='Config file path')
        return parser.parse_args()

    def loadJsonConfig(self, file):
        """Load configuration file"""
        self.debug('Loading config file...')
        return json.load(open(file, 'r'))

