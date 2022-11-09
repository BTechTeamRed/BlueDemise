import os

txtfiles = os.listdir('..\\Game\\Assets\\Scripts')
headers = []
scriptNames = []

for file in txtfiles:
    if file.endswith(".h"):
        headers.append(file)

for header in headers:
    scriptNames.append(header.replace('.h', ''))
    
d = {}
includes = ''
scriptCompares = ''

for script in scriptNames:
    includes += '#include '
    includes += script
    includes += '.h\n'

    scriptCompares += f'        if (scriptName == "{script}")\n'
    scriptCompares += f'        {{\n'
    scriptCompares += f'            entity.addComponent<ScriptComponent>().linkBehavior<{script}>();\n'
    scriptCompares += f'        }}\n'

d['includes'] = includes
d['compareScripts'] = scriptCompares
with open("scriptSerializerTemplate.cpp", 'r') as ftemp:
    templateString = ftemp.read()
with open("output.cpp", 'w') as f:
    f.write(templateString.format(**d))