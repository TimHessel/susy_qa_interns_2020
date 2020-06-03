import random
import matplotlib.pyplot as plt
import numpy as np

def prediction_maker(data_label, a)
    b = 1
    len = data_label.shape[0]
    predictions = np.zeros(len)
    for i in range(len) :
        if data_label[i] == 1 :
            predictions[i] = random.betavariate(a, b)
        else :
            predictions[i] = random.betavariate(b, a)
    return predictions
