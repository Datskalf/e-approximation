import json, pathlib, os, math
import matplotlib.pyplot as plt

def readFile():
    filePath = os.path.join(pathlib.Path(__file__).parent, 'Results.json')

    with open(filePath, 'r') as f:
        content = f.read()
    
    jsonFile = json.loads(content)
    return jsonFile

def drawGraph(xIntervals, yPoints):
    plt.figure(figsize=(16, 6))

    plt.xlabel('Estimation after x iterations')
    plt.ylabel('Entimation')
    plt.title('e estimation through random numbers')
    plt.plot(xIntervals, yPoints, label='Estimation')

    plt.axhline(y=math.e, color='r', linestyle='dashed')

    

    plt.show()

if __name__ == '__main__':
    fileContent = readFile()[-1]
    intervalCount = fileContent['Update frequency']
    simulationCount = fileContent['Loop count']

    entries = fileContent['Results']
    intervals = [(fileContent['Update frequency']*(i+1)) for i in range(len(entries))]

    drawGraph(intervals, entries)