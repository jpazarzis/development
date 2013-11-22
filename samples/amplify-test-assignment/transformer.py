#!/usr/bin/python
'''
Author: John Pazarzis
Thu Nov 21 21:17:45 EST 2013

transformer.py

This program implements a command line utility accepting student enrollment data
in a specific format transforming it to a different one.

By default, this program handles CSV and XML formats while makes it easy to add
new format with minimum changes. 

Use:  
    The command line should look like this:

    transformer.py <input> <output>

    input / output: 
        must be either an xml file with the extention .xml 
        or a csv file with the extention csv

    at this point only xml and csv formats are supported

    to add a new formating option (xyz for example) you need to implement a 
    couple of functions:

    xyz2obj(filename)

    and 

    xyz2csv(obj)  and or xyz2xml(obj)

    following as a sample the pairs:

    xml2obj - csv2obj

    and 

    obj2xml - obj2csv

    and register them like this:

    readers = {'.xml':xml2obj, '.csv': csv2obj, '.xyz': xyz2obj}
    writers = {'.xml':obj2xml, '.csv': obj2csv, '.xyz': xyz2csv}


Implementation Details:

    The input data (in any format) are converted to an object which is 
    format neutral. 

    The xxx2obj functions are doing exactly this. They receive a filename
    containing the data returning a Python object implementing a tree like
    representation of the data that can now be saved in any format having a
    corresponding transformation function (xxx2xml or xxx2csv for example)

Note:

    There are some discrepancies between the two provided sample files, for
    example the csv file contains a field called student_grade that does not
    appear in the xml while the xml contains school and grade which are not part
    of the csv. Since it is undefined how to process them, I chose to hard code
    their default values.
'''

import sys
import csv
import xml.etree.cElementTree as ET
import os.path

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
        if len(tokens) >= index+1:
            student.student_grade = tokens[index]
        else:
            student.student_grade = ""

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

def obj2csv(obj):
    def get_teachers(teachers):
            tokens =[]
            if teachers and len(teachers) >= 1:
                tokens.extend( [teachers[0].id, teachers[0].last_name, teachers[0].first_name] )
            if teachers and len(teachers) > 1:
                tokens.extend( [teachers[1].id, teachers[1].last_name, teachers[1].first_name] )
            return tokens + [' '] * (6 -len(tokens))

    grade = obj.grade[0]
    strg = 'classroom id, classroom_name, teacher_1_id, teacher_1_last_name,\
    teacher_1_first_name, teacher_2_id, teacher_2_last_name,\
    teacher_2_first_name, student_id, student_last_name, student_first_name,\
    student_grade\n'

    for classroom in grade.classroom:
        students, teachers = None, None
        if hasattr(classroom, 'student'):
            students = classroom.student
        if hasattr(classroom, 'teacher'):
            teachers = classroom.teacher
            
        if students:
                for student in classroom.student:
                    tokens = [classroom.id, classroom.name]
                    tokens.extend(get_teachers(teachers))
                    tokens.extend([student.id, student.last_name, student.first_name])
                    if hasattr(student,'student_grade'):
                            tokens.append(student.student_grade)
                    else:
                            tokens.append(' ')
                    strg += ','.join(tokens) + '\n'       

        if not students:     
            tokens = [classroom.id, classroom.name]
            tokens.extend(get_teachers(teachers))
            tokens.extend([''] * 4)
            strg += ','.join(tokens) + '\n'
    return strg
    

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print 'please provide in the command line the input and the output filenames'
        sys.exit(1)

    readers = {'.xml':xml2obj, '.csv': csv2obj}
    writers = {'.xml':obj2xml, '.csv': obj2csv}

    try:
            input_filename = sys.argv[1]
            output_filename = sys.argv[2]

            input_extension = os.path.splitext(input_filename)[1]
            output_extension = os.path.splitext(output_filename)[1]

            if input_extension not in readers:
                raise Exception("Extension: " + input_extension + " is not supported")

            if output_extension not in writers:
                raise Exception("Extension: " + output_extension + " is not supported")

            if input_extension == output_extension:
                raise Exception("Cannot convert to the same type.")

            reader = readers[input_extension]
            writer = writers[output_extension]

            with open(output_filename, 'w') as f:
                f.write(writer(reader(input_filename)))

    except Exception as ex:
        print 'Error:',ex
