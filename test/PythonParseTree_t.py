#!/usr/bin/env python
#  This compares the content of two Python files,
#  ignoring trivial differences like order within
#  dictionaries or escape characters

from sys import argv
from sys import exit
from sys import *


import unittest
import libFWCoreParameterSet

class PythonParseTree_t(unittest.TestCase):

    def setUp(self):
        self.tree = libFWCoreParameterSet.PythonParseTree("Replace.cfg")

    def testValue(self):
        self.assertEqual(self.tree.value("rome.date"), '100')
        self.assertEqual(self.tree.values("rome.legion.cohorts"), 
                         ['480','480','480','480','480','480','480','480','480','480'])
        # make sure vectors and single entries are separate
        self.assertRaises(RuntimeError, self.tree.value, "rome.legion.cohorts")
        self.assertRaises(RuntimeError, self.tree.values, "rome.date")

    def testReplace(self):
        self.tree.replaceValue("Y2.s", "serious")
        self.tree.replaceValue("benHur.chariot.hubcaps", "spinners")
        self.tree.replaceValues("rome.asians", ["byzantines"])
        self.tree.process()
        self.assertEqual(self.tree.value("Y2.s"), "serious")
        self.assertEqual(self.tree.value("benHur.chariot.hubcaps"), "spinners")
        self.assertEqual(self.tree.values("rome.asians"), ["byzantines"])

    def testModules(self):
        # these should throw if the correct module is not found
        self.tree.modules().index("rome")
        self.tree.modulesOfType("looper").index("benHur")
        self.tree.modulesOfType("source").index("SlaveLabor")
        # should fail
        self.assertRaises(ValueError, self.tree.modulesOfType("looper").index, "rome")

if __name__=='__main__':
    unittest.main()


