from os import walk
import re

pattern = re.compile(r'''([0-9]+\.[0-9]+f)''')

for _, _, filenames in walk('.'):
    for filename in filenames:
        if filename.endswith('.h'):
            content = None
            with open(filename, 'r') as file:
                content = file.read()

            content = re.sub(pattern, '"$1"_fx', content)

            break