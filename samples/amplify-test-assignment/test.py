#!/usr/bin/python
'''
Author: John Pazarzis
Thu Nov 21 21:17:45 EST 2013

This program implements a command line utility accepting student enrollment data
in a specific format transforming it to a different one.

By default, this program handles CSV and XML formats while makes it easy to add
new format with minimum changes. 

Use:  
    In the command line provide the input using the -i [filename] flag and the
    output using the -o [filename]. Note that the extension of both input and
    output files specifies the type of transformation. An unsupported
    transformation will cause a warning while a valid one will execute the
    transformation.
ls
'''



import sys
import csv
import xml.etree.cElementTree as ET


def xml2obj(filename):
    '''
    receives an xml filename returning a python object reflecting the structure
    of the xml code. xml attributes are becoming member variables that can be
    accessed by their name while xml keys are becoming entries in list objects
    that can again be accessed by name...
    '''
    def node_to_object(node):
            obj = type(node.tag, (object,), {})
            obj.tag=node.tag.strip().replace('-','_')
            obj.text=''
            if node.text is not None:
                obj.text = node.text.strip()
            for attrname in node.attrib:
                setattr(obj,attrname,node.attrib[attrname])
            for child in node:
                c = node_to_object(child)
                if not hasattr(obj,c.tag):
                    setattr(obj,c.tag,[])
                getattr(obj,c.tag).append(c)
            return obj
    return node_to_object(ET.ElementTree(file=filename).getroot())

def csv2obj(filename):
   '''
   For simplicity I will assume a predefined 'standard' csv structure complying
   to a specific layout exactly matching the sample file. If we need more
   flexibility we can parse the first (header) line to create a dynamic layout
   following some sort of convention. 

   Since it is not clear to me how school and grade should be handled since they
   are not part of the csv layout I will have to use default values for now.
   '''

   root = type("school", (object,), {'id':"100", 'name':"WGen School", 'grade':[]})
   grade = type("grade", (object,), {'id':"100", 'classroom':[]})
   root.grade.append(grade)

   classrooms = {}

   for i, tokens in enumerate(csv.reader(open(filename,'rb'))):
        if i == 0 : 
            continue # skip first line
        index = 0    
        classroom = type("classroom", (object,),{}) 
        classroom.id = tokens[index]
        index += 1
        classroom.name = tokens[index]
        index += 1

        if classroom.id not in classrooms:
            classroom.teacher, classroom.student = [], []
            classrooms[classroom.id] = classroom
        else:
            classroom = classrooms[classroom.id]

        for i in range(2):
            teacher = type("teacher", (object,),{}) 
            teacher.id = tokens[index].strip()
            index += 1
            teacher.last_name = tokens[index]
            index += 1
            teacher.first_name = tokens[index]
            index += 1
            if len(teacher.id) > 0:
                if teacher.id not in [t.id for t in classroom.teacher]:
                    classroom.teacher.append(teacher)

        student = type("student", (object,),{}) 
        student.id = tokens[index].strip()
        index += 1
        student.last_name = tokens[index]
        index += 1
        student.first_name = tokens[index]
        index += 1
        student.student_grade = tokens[index]

        if len(student.id) >0:
            classroom.student.append(student)

   grade.classroom = classrooms.values()
   return root

def obj2xml(obj, left_margin_count = 0):
    left_margin = ' ' *  left_margin_count
    strg = left_margin + '<{0} '.format(obj.__name__)
    contents = filter(lambda a: not a.startswith('__'), dir(obj))
    for c in contents:
        a = getattr(obj,c)
        if isinstance(a, str):
            strg += ' {0}="{1}" '.format(c, a)
    has_children = False
    for c in contents:
        a = getattr(obj,c)
        if isinstance(a, list):
            if not has_children:
                has_children = True
                strg += '>\n'
            for x in a:
                    strg += left_margin + obj2xml(x, left_margin_count + 4)
    if has_children:
        strg += left_margin + '</{0}>\n'.format(obj.__name__)
    else:
        strg +='/>\n'
    return strg

    
    

def test_xml():
    a = xml2obj('sample_data.xml')
    print a.grade[0].id
    for cr in a.grade[0].classroom:
        print cr.id, cr.name


if __name__ == '__main__':

    a = csv2obj('sample_data.csv')
    print obj2xml(a)











