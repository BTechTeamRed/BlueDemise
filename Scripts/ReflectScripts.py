import os
import shutil
import sys

#Gather all scripts
txtfiles = os.listdir(f'{sys.argv[1]}\\Game\\Assets\\Scripts')
headers = []
scriptNames = []

#Get all script names
for file in txtfiles:
    shutil.copy(f'{sys.argv[1]}\\Game\\Assets\\Scripts\\{file}', f'{sys.argv[1]}\\Engine\\src\\Engine\\Scripts')
    if file.endswith(".h"):
        headers.append(file)

#Get all script names without extension
for header in headers:
    scriptNames.append(header.replace('.h', ''))
    
d = {}
includes = ''
scriptCompares = ''

#Create cpp file
for script in scriptNames:
    includes += '#include "Engine/Scripts/'
    includes += script
    includes += '.h"\n'

    scriptCompares += f'        if (scriptName == "{script}")\n'
    scriptCompares += f'        {{\n'
    scriptCompares += f'            entity.addComponent<ScriptComponent>().linkBehavior<{script}>();\n'
    scriptCompares += f'        }}\n'

d['includes'] = includes
d['compareScripts'] = scriptCompares
with open(f"{sys.argv[1]}\\Scripts\\scriptSerializerTemplate.cpp", 'r') as ftemp:
    templateString = ftemp.read()
with open(f"{sys.argv[1]}\\Engine\\src\\Engine\\ResourceManagement\\ScriptSerializer.cpp", 'w') as f:
    f.write(templateString.format(**d))