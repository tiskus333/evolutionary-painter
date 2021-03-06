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
        self.save_result_image = False
        self.execution_timeout = 0

    @staticmethod
    def generate_EvolAlg(filename=None,
                         size: int = 30,
                         genes_count: int = 500,
                         max_generation=0,
                         window_visibility=True,
                         save_result=False) -> EvolAlg:

        if filename is None:
            completed = subprocess.run(
                "zenity --file-selection", shell=True, stdout=subprocess.PIPE, universal_newlines=True)  # TODO check it for Windows
            filename = completed.stdout[0:-1]

        evolution = EvolAlg(filename, size, genes_count,
                            max_generation, window_visibility, save_result)
        return evolution

    def run(self):
        self._evol = self.generate_EvolAlg(
            self.filename, self.size, self.genes_count, self.max_generation, self.has_window, self.save_result_image)

        self._stats = StatsObserver(self.execution_timeout)
        self._stats.setObserved(self._evol)
        ter = TerminalObserver()
        ter.setObserved(self._evol)

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

    def test_multiple_times(self, changed_field: str, min_value, step_size=10, number_of_steps=1, number_of_tries=1):
        '''changed can be either "size" or "genes_count"
        '''
        setting_func = None
        if changed_field == "size":
            setting_func = self._set_size
        elif changed_field == "genes_count":
            setting_func = self._set_genes_count
        else:
            raise ValueError

        results = dict()

        for number in range(number_of_steps + 1):
            current_value = min_value + number*step_size
            setting_func(current_value)
            multiple_fits = []
            for num in range(number_of_tries):
                self.run()
                gen, fit = self.get_data()
                multiple_fits.append(fit)

            if "gen" not in results.keys():
                results["gen"] = gen
            results[current_value] = self._list_avg(multiple_fits)

        return results

    def _set_size(self, number: int) -> int:
        self.size = number
        return self.size

    def _set_genes_count(self, number: int) -> int:
        self.genes_count = number
        return self.genes_count

    @staticmethod
    def _list_avg(lists_of_lists):
        sums = [sum(x) for x in zip(*lists_of_lists)]
        result = []
        for s in sums:
            result.append(s/len(lists_of_lists))
        return result
