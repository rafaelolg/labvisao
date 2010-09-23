#!/usr/bin/env python
# -*- coding: utf-8 -*-

# calcula_precision_recall.py

NUM_IMGS=120
PRECISION = 0.001


def tp_fp_tn_fn(elements_of, similarity, threshold):
    print (threshold)
    true_positive = 0
    false_positive = 0
    true_negative = 0
    false_negative = 0

    classes = [linha[0] for linha in similarity]
    for classe in classes:
        for element in range(1, NUM_IMGS):
            if similarity[classe][element] >= threshold:
                #positive
                if element in elements_of[classe]:
                    #true
                    true_positive += 1
                else:
                    false_positive += 1
            else:
                #negative
                if element in elements_of[classe]:
                    #false
                    false_negative += 1
                else:
                    true_negative += 1
    
    especificidade = true_negative / (true_negative + false_positive)
    sensitividade = true_positive / (true_positive + false_negative)
    return (true_positive, false_positive, 
            true_negative,
            false_negative,
            especificidade,
            sensitividade)


if __name__ == '__main__':
    import sys

    metodos = ["0", '1', '2']

    elements_of = {}
    gtfile = open(sys.argv[1], 'r')
    for linha in gtfile:
        index = linha.split()
        elements_of[index[0]] =[int(i) for i in index[1:]]

    for m in metodos:
        file = open('similaridades_%s_.csv' % m, 'r')
        medidas = open('medidas_%s.dat' % m, 'w')
        similaridade = {}
        for linha in file:
            row = linha.split(',\t')
            similaridade[row[0]] = [float(val) for val in row[1:]]
        
        th = 0
        medidas.write('THRESH TP FP TN FN ESPECIFICIDADE SENSITIVIDADE\n')
        while th < 1:
            med = ["%f" % th] +["%f" %  med for med in tp_fp_tn_fn(elements_of, similaridade, th)]
            medidas.write(' '.join(med))
            medidas.write('\n')
            th += PRECISION
        medidas.close()
