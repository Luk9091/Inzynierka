import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('data3.csv', sep=', ')

def findConstantValue(data):
    sum = 0
    for i in range(len(data)):
        sum += data[i]
    return sum / len(data)

def removeNoiseInSample(dataFrame: pd.DataFrame, sampleSize: int = 25):

    min = {"angle": [], "x": [], "y": [], "z": []}
    max = {"angle": [], "x": [], "y": [], "z": []}
    avg = {"angle": [], "x": [], "y": [], "z": []}

    for start in range(0, len(dataFrame), sampleSize):
        min["angle"].append(dataFrame['angle'][start])
        max["angle"].append(dataFrame['angle'][start])
        avg["angle"].append(dataFrame['angle'][start])

        min["x"].append(dataFrame['x'][start])
        min["y"].append(dataFrame['y'][start])
        min["z"].append(dataFrame['z'][start])

        max["x"].append(dataFrame['x'][start])
        max["y"].append(dataFrame['y'][start])
        max["z"].append(dataFrame['z'][start])

        avg["x"].append(0)
        avg["y"].append(0)
        avg["z"].append(0)

        for i in range(sampleSize):
            if dataFrame['x'][start + i] < min["x"][-1]:
                min["x"][-1] = dataFrame['x'][start + i]
            if dataFrame['x'][start + i] > max["x"][-1]:
                max["x"][-1] = dataFrame['x'][start + i]
            avg["x"][-1] += dataFrame['x'][start + i]

            if dataFrame['y'][start + i] < min["y"][-1]:
                min["y"][-1] = dataFrame['y'][start + i]
            if dataFrame['y'][start + i] > max["y"][-1]:
                max["y"][-1] = dataFrame['y'][start + i]
            avg["y"][-1] += dataFrame['y'][start + i]

            if dataFrame['z'][start + i] < min["z"][-1]:
                min["z"][-1] = dataFrame['z'][start + i]
            if dataFrame['z'][start + i] > max["z"][-1]:
                max["z"][-1] = dataFrame['z'][start + i]
            avg["z"][-1] += dataFrame['z'][start + i]

        avg["x"][-1] /= sampleSize
        avg["y"][-1] /= sampleSize
        avg["z"][-1] /= sampleSize

    return min, max, avg


def calculateAngle(data):
    angle = []
    for i in range(len(data['x'])):
        angle.append(np.arctan2(data['y'][i], data['x'][i]) + 270/360 * 2 * np.pi)
        if (angle[-1] > 2*np.pi):
            angle[-1] -= 2*np.pi
        angle[-1] = angle[-1] * 180 / np.pi 
        angle[-1] += (4.0 + (26.0 / 60.0)) / (180 / np.pi)
    return angle

def average(data):
    angle = []
    x = []
    y = []
    z = []

    iteration = []
    for i in list(data["angle"]):
        if data["angle"][int(i)] not in angle:
            angle.append(data["angle"][int(i)])
            x.append(data["x"][int(i)])
            y.append(data["y"][int(i)])
            z.append(data["z"][int(i)])
            iteration.append(1)
        else:
            index = angle.index(data["angle"][int(i)])
            x[int(index)] = (x[int(index)] + data["x"][int(i)])
            y[int(index)] = (y[int(index)] + data["y"][int(i)])
            z[int(index)] = (z[int(index)] + data["z"][int(i)])
            iteration[index] += 1

    for i in range(len(angle)):
        x[int(i)] = (x[int(i)] / iteration[int(i)])/2048
        y[int(i)] = (y[int(i)] / iteration[int(i)])/2048
        z[int(i)] = (z[int(i)] / iteration[int(i)])/2048

    return {"angle": angle, "x": x, "y": y, "z": z}




Max, Min, Avg = removeNoiseInSample(df, 25)
offset = {'x': findConstantValue(Avg['x']), "y": findConstantValue(Avg['y'])}
Avg['x'] = Avg['x'] - offset['x']
Avg['y'] = Avg['y'] - offset['y']

Average = average(Avg)


print(f"x: max: {max(Max['x'])}, min: {min(Min['x'])}")
print(f"y: may: {max(Max['y'])}, min: {min(Min['y'])}")
print(f"Avg x: max: {max(Avg['x'])}, min: {min(Avg['x'])}")
print(f"Avg y: max: {max(Avg['y'])}, min: {min(Avg['y'])}")

plt.figure("Linear")
plt.scatter(df['angle'], df['x'], s=2) 
plt.scatter(df['angle'], df['y'], s=2)
plt.scatter(Avg['angle'], Avg['x'], label = "x", s=2)
plt.scatter(Avg['angle'], Avg['y'], label = "y", s=2)
plt.legend(["measure x", "measure y", "optimize x", "optimize y"])
plt.grid()

plt.figure("Polar")
plt.scatter(df['x']/2048, df['y']/2048)
plt.scatter(Avg['x']/2048, Avg['y']/2048)
plt.scatter(Average['x'], Average['y'])
plt.legend(["measure", "optimize"])
plt.grid()

plt.figure("Angle")
plt.scatter(df['angle'], calculateAngle(df))
plt.scatter(Avg['angle'], calculateAngle(Avg))
plt.scatter(Average["angle"], calculateAngle(Average))
plt.legend(["measure", "optimize"])
plt.grid()

plt.show()
