from FordFulkerson import FordFulkerson
from FlowNetwork import FlowNetwork
from FlowEdge import FlowEdge

#V = 10
#E = 100
#G = FlowNetwork.RandomNetwork(V,E)
G = FlowNetwork(6)
G.addEdge(FlowEdge(0,1,2.0))
G.addEdge(FlowEdge(0,2,3.0))
G.addEdge(FlowEdge(1,3,3.0))
G.addEdge(FlowEdge(1,4,1.0))
G.addEdge(FlowEdge(2,3,1.0))
G.addEdge(FlowEdge(2,4,1.0))
G.addEdge(FlowEdge(3,5,2.0))
G.addEdge(FlowEdge(4,5,3.0))


s = 0
t = G.V()-1

print(G)

# compute maximum flow and minimum cut
maxflow = FordFulkerson(G, s, t)
print("Max flow from",s,"to",t)

for v in range(G.V()):
    for e in G.adj(v):
        if ((v == e.From()) and e.flow() > 0):
            print(" ",e)

# print min-cut
cut = []
for v in range(G.V()):
    if (maxflow.inCut(v)):
        cut.append(v)
print("Min cut:",cut)
print("Max flow value =",maxflow.value())
