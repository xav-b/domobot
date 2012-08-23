#!/usr/bin/python
# -*- coding: utf-8 -*-


from ProcessInfo import ProcessInfo
import pylab
import time
import tempfile
import threading
import sys


class ProcessStatistics():
    """ Stocke les statistiques de plusieurs processus en mémoire """
    def __init__(self, name_list, interval):
        """ Init process list
        @param pid_list : list des identifiants des processus à
        @surveiller
        @interval : intervalle entre chaque mesure
        """
        # init vars
        self.values = {}
        # Instanciation de ProcessInfo et lancement de la récupération des valeurs
        self.processes_info = []
        for name in name_list:
            #pid = int(pid)
            pi = ProcessInfo(name, interval, self._store_data)
            self.values[pi.pid] = []
            pi_process = threading.Thread(None, pi.start, None, (), {})
            pi_process.start()
        # Génération des graphiques toutes les 30 secondes
        loop_charts = True
        while loop_charts == True:
            time.sleep(30)
            self.generate_charts()

    def _store_data(self, pid, data):
        """ Store data
        @param pid : identifiant du processus
        @param data : dictionnaire de données
        """
        print "pid : %s, data : %s" % (pid, data)
        self.values[pid].append({"time" : time.time(), "data" : data})

    def generate_charts(self):
        """ Génération des graphiques """
        for pid in self.values:
            time_range = []
            data = {}
            for item in self.values[pid]:
                print "- %s" % item
                time_range.append(item["time"])
                for elt in item["data"]:
                    #if hasattr(data, elt) == False:
                    #   data[elt] = []
                    try:
                        data[elt].append(item["data"][elt])
                    except KeyError:
                        data[elt] = [item["data"][elt]]
            self._generate_chart(time_range, data["cpu_percent"],
            "Cpu usage", self._get_filename(pid, "cpu_percent"))
            self._generate_chart(time_range, data["memory_rss"],
            "Memory : RSS", self._get_filename(pid, "memory_rss"))
            self._generate_chart(time_range, data["memory_vsz"],
            "Memory : VSZ", self._get_filename(pid, "memory_vsz"))
            self._generate_chart(time_range, data["memory_percent"],
            "Memory usage", self._get_filename(pid, "memory_percent"))

    def _get_filename(self, pid, name):
        """ Création du nom de fichier
        @param pid : identifiant du processus
        @param name : nom de la valeur
        """
        return "%s/chart-%s-%s.png" % (tempfile.gettempdir(), pid, name)

    def _generate_chart(self, time_range, values, label, filename):
        """ Génration d’un graphique
        @param time_range : axe des X
        @param values : valeurs
        @param label : libellé à afficher
        @param filename : nom du fichier à générer
        """
        fig = pylab.figure()
        ax = fig.add_subplot(111)
        ax.plot( time_range, values, '.-', label = label)
        pylab.title(label)
        fig.savefig(filename, dpi=(640/8))


if __name__   == "__main__":
    my_stats = ProcessStatistics(sys.argv[1:], 5)
