#!/usr/bin/python3 -i

import os
import subprocess
import sys
import time

import matplotlib.pyplot as plt

module_path = os.path.dirname(os.path.realpath(__file__)) + "/build/"
sys.path.append(module_path)

try:
    from bindings import *
    print("Library imported")
except:
    print("Something went wrong during import")
    print("__file__ : " + __file__)
    print("sys.path: " + str(sys.path))


class BindWrapper(object):
    """
    Simple wrapper for EvolAlg and StatsObserver

    """

    def __init__(self):
        self._evol = None
        self._stats = None

        # Init default values for EvolAlg
        self.filename = None
        self.size = 30
        self.genes_count = 500
        self.max_generation = 0
        self.has_window = True

    @staticmethod
    def generate_EvolAlg(filename=None,
                         size: int = 30,
                         genes_count: int = 500,
                         max_generation=0,
                         window_visibility=True) -> EvolAlg:

        if filename is None:
            completed = subprocess.run(
                "zenity --file-selection", shell=True, stdout=subprocess.PIPE, universal_newlines=True)  # TODO check it for Windows
            filename = completed.stdout[0:-1]

        evolution = EvolAlg(filename, size, genes_count,
                            max_generation, window_visibility)
        return evolution

    def run(self):
        self._evol = self.generate_EvolAlg(
            self.filename, self.size, self.genes_count, self.max_generation, self.has_window)

        self._stats = StatsObserver()
        self._stats.setObserved(self._evol)

        self._evol.run()

    def get_data(self):
        results = self._stats.get()
        fit = results.fit()
        gen = results.gen()
        return gen, fit

    def draw_plot(self):
        fig, plot = plt.subplots()
        results = self._stats.get()
        fit = results.fit()
        gen = results.gen()
        plot.plot(gen, fit)
        plt.show()

