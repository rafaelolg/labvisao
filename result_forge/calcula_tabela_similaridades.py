#!/usr/local/bin/python
# -*- coding: utf-8 -*-

# DESCRIÇÃO
# Esse programa constroi as tabelas de Precision x Recall e ROC
# (Recall (TPR) x FPR)
#
# Para rodar o programa é necessário:
# 1) arquivo de ground truth no formato:
#    - imagemDeBusca ind1 ind2 ... indN
# 2) imagens a serem buscadas devem possuir uma raiz do nome, seguido
#    por um número (seu indice).
#    Exemplo: dir/img.001.jpg, tem dir/img como raiz dir/img e indice 001

# Exemplo:
# python pr.py gt.txt busca/image.

import sys
import subprocess

def precRec (imgRef, imgDB, gt, MAX=10, firstImg=1, lastImg=120, metodo='ERRRo', similaridade=[]):
    vals = []
    for i in range (firstImg, lastImg+1):
        value = float(subprocess.check_output(["./calcula_similaridade_entre", imgRef, "%s%04d.jpg"%(imgDB,i), metodo]))
        print('%s<->%s = %s\t(%s)' %(imgRef, "%s%04d.jpg"%(imgDB,i), value, metodo))
        vals.append(float(value))
    simi = [imgRef] + ["%f"%v for v in vals]
    similaridade.append(simi)





####################################################
##      Main Program
######
        
if len(sys.argv) != 3:
    print ("Usage: pr groundTruth.txt imageDir")
    sys.exit(0)

fin = open(sys.argv[1], 'r')
iDir = sys.argv[2]

metodos = ['0', '1', '2']
similaridade = {}
for metodo in metodos:
    similaridade[metodo] = []

for line in fin:
    index = line.split()
    for metodo in metodos:
    print(index)
        precRec(index[0], iDir, index[1:], 100, metodo=metodo, similaridade=similaridade[metodo])


for metodo in metodos:
    tabela = open("similaridades_%s_.csv"%(metodo), 'w')
    for linha in similaridade[metodo]:
        txt = ',\t'.join(linha)
        txt += '\n'
        tabela.write(txt)
    tabela.close()
