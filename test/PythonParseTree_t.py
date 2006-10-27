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
        self.pset = libFWCoreParameterSet.ParameterSet()

    def testValue(self):
        self.assertEqual(self.tree.value("rome.date"), '100')
        self.assertEqual(self.tree.values("rome.legion.cohorts"), 
                         ['480','480','480','480','480','480','480','480','480','480'])
        self.assertEqual(self.tree.typeOf("rome.legion.cohorts"), "vint32");
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

    def testPSet(self):
        self.pset.addInt32(True, "dwarves", 7)
        self.assertEqual(self.pset.getInt32(True, "dwarves"), 7)
        l = [1,2,3,4,5,6,7]
        self.pset.addVInt32(True, "numbers" , l)
        self.assertEqual(self.pset.getVInt32(True, "numbers")[6], 7)
 
        self.pset.addString(False, "villain", "Skeletor")
        self.assertEqual(self.pset.getString(False, "villain"), "Skeletor")
        self.pset.addVString(True, "heroes", ["He-Man", "She-Ra"])
        self.assertEqual(self.pset.getVString(True, "heroes")[1], "She-Ra")
        #check both trackedness
        #self.assertRaises(RuntimeError, self.pset.getVString(False, "heroes"))
        #self.assertRaises(RuntimeError, self.pset.getString(True, "villain"))
        
    def testInputTag(self):
        t1 = libFWCoreParameterSet.InputTag("label", "instance")
        t2 = libFWCoreParameterSet.InputTag("label2:instance2")
        self.pset.addInputTag(True, "t1", t1)
        self.pset.addInputTag(False, "t2", t2)
        self.assertEqual(self.pset.getInputTag(True, "t1").label(), "label")
        self.assertEqual(self.pset.getInputTag(False, "t2").label(), "label2")

    def testFileInPath(self):
        p = "FWCore/ParameterSet/interface/ParameterSet.h"
        f = libFWCoreParameterSet.FileInPath(p)
        self.pset.addFileInPath(True, "f", f)
        f2 = self.pset.getFileInPath(True, "f")
        self.assertEqual(f2.relativePath(), p)

    def testNestedPSet(self):
        shows = libFWCoreParameterSet.ParameterSet()
        dukes = ["Bo", "Luke"]
        shows.addVString(True, "dukes", dukes)
        shows.addInt32(True, "generalLee", 1)
        self.pset.addPSet(True, "tvshows", shows)
        p2 = self.pset.getPSet(True, "tvshows")
        self.assertEqual(p2.getVString(True, "dukes"), ["Bo", "Luke"])
        #self.assertEqual(p2.getParameter(True, "generalLee"), 1)
 
if __name__=='__main__':
    unittest.main()


