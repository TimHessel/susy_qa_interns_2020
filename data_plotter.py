import prepare_data
import matplotlib.pyplot as plt

LepPt = sig["LepPt"]
Weight = sig["Weights"]

plt.plot(LepPt, Weights)
x.label("LepPt")
y.label("Weights")
plt.title("LepPt Normalized by Nevt")
plt.show()
