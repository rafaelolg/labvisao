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

def precRec (imgRef, imgDB, gt, MAX=10, firstImg=1, lastImg=120, TP="ERRO", TN="ERRO",
        FP="ERRO", FN="ERRO", metodo='ERRO'):
    fOut = open("%s.%s.tabela"%(imgRef, metodo), 'w')
    fOut.write("TH, TP, TN, FP, FN, P, R, Acu, FPR\n")
    threshold = 0.01
    cont = firstImg

    # compute values
    vals = []
    for i in range (firstImg, lastImg+1):
        img_target = "%s%04d.jpg"%(imgDB,i)
        value = float(subprocess.check_output(["./calcula_similaridade_entre", imgRef, img_target , "%s" % metodo]))
        vals.append(float(value))
    for th in range (MAX):
        for i in range (lastImg):
            value = vals[i]
            sti = "%03d"%i
            thf = float(th)/MAX
            if value >= thf:
                if sti in gt:
                    TP += 1
                else:
                    FP += 1
            else:
                if sti in gt:
                    FN += 1
                else:
                    TN += 1
        # print TP, FP, FN, TN
        p = r = FPR = 0.0
        if (TP+FP) > 0:
            p = float (TP) / (TP+FP)
        if (TP+FN) > 0:    
            r = float (TP) / (TP+FN) # True positive rate
        if (FP+TN) > 0:
            FPR = float(FP) / (FP+TN)
        acu = float(TP + TN)/lastImg
        fOut.write("%.3f, %d, %d, %d, %d, %7.3f, %7.3f, %7.3f, %7.3f\n" %( thf, TP, TN, FP, FN, p, r, acu, FPR ))
#        print("%.3f, %d, %d, %d, %d, %7.3f, %7.3f, %7.3f, %7.3f\n" %( thf, TP, TN, FP, FN, p, r, acu, FPR ))
    fOut.close()
    print ("Resultado em: %s.tabela"%(imgRef))

####################################################
##      Main Program
######
if len(sys.argv) != 3:
    print ("Usage: pr groundTruth.txt imageDir")
    sys.exit(0)

fin = open(sys.argv[1], 'r')
iDir = sys.argv[2]

for line in fin:
    index = line.split()
    print ("Processando imagem " +  index[0])

    TP =[0, 0, 0]
    TN =[0, 0, 0]
    FP =[0, 0, 0]
    FN =[0, 0, 0]
    for metodo in [0, 1, 2]:
        precRec(index[0], iDir, index[1:], 100,
                TP=TP[metodo],
                TN=TN[metodo],
                FP=FP[metodo],
                FN=FN[metodo],
                metodo=metodo)
