#
# Ferramenta para mapear os endereços com mais facilidade
# Autor: Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>
#

first = int(input('first address: '), 16)
second = int(input('second address: '), 16)
ver = input('Warzone version: ')
diff = second - first
current = first

print(f'offset is {diff} or {hex(diff)} in base 16')
for i in range(0, 11):
		print('{%d, 0, 0, 0, %s, 0, 0, 0, 0, 0, %s},' % (i, hex(current), ver))
		current += diff
