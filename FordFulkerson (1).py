import FlowEdge
from collections import deque

FLOATING_POINT_EPSILON = 1E-11

class FordFulkerson:

    """
    Original Authors:
    Robert Sedgewick
    Kevin Wayne

    Adapted by Varick Erickson
    """
    def __init__(self,G,s,t):
        #print(G)
        self.__value = 0.0
        self.__V = G.V()
        self.__validate(s)
        self.__validate(t)

        if (s == t):
            raise Exception("Source equals sink")
        if (not self.__isFeasible(G,s,t)):
            raise Exception("Initial flow is infeasible")

        self.__value = self.__excess(G,t)
        while (self.__hasAugmentingPath(G, s, t)):
            # compute bottleneck capacity
            bottle = float('inf');
            v = t
            while (v != s):
                #print(v,s)
                bottle = min(bottle, self.__edgeTo[v].residualCapacityTo(v))
                v = self.__edgeTo[v].other(v)

            # augment flow
            v = t
            while (v != s):
                self.__edgeTo[v].addResidualFlowTo(v, bottle)
                v = self.__edgeTo[v].other(v)
                
            self.__value += bottle;

        # assert check(G,s,t)

    """
    Returns the maximum flow.
    """
    def value(self):
        return self.__value

    def inCut(self,v):
        self.__validate(v)
        return self.__marked[v]

    def __validate(self,v):
        if (v < 0 or v >= self.__V):
            raise Exception("vertex " + str(v) + " is not between 0 and " + str(self.__V-1));

    def __hasAugmentingPath(self, G, s, t):
        self.__edgeTo = G.V()*[None];
        self.__marked = G.V()*[None];

        # bfs
        queue = deque()
        queue.append(s)
        self.__marked[s] = True;
        while (len(queue)>0 and not self.__marked[t]):
            v = queue.pop()

            for e in G.adj(v):
                w = e.other(v)

                # if residual capacity from v to w
                if (e.residualCapacityTo(w) > 0):
                    if (not self.__marked[w]):
                        self.__edgeTo[w] = e
                        self.__marked[w] = True
                        queue.append(w)

        # Augmenting path?
        return self.__marked[t]

    # return excess flow at vertex v
    def __excess(self, G, v):
        excess = 0.0
        for e in G.adj(v):
            if (v == e.From()):
                excess -= e.flow();
            else:
                excess += e.flow();
        return excess

    # return excess flow at vertex v
    def __isFeasible(self, G, s, t):
        #print("in isFeasible...")
        # check that capacity constraints are satisfied
        for v in range(G.V()):
            for e in G.adj(v):
                if (e.flow() < -FLOATING_POINT_EPSILON or
                    e.flow() > e.capacity() + FLOATING_POINT_EPSILON):
                    print("Edge does not satisfy capacity constraints:",e)
                    return False
        
        # check that net flow into a vertex equals zero, except at source and sink
        if (abs(self.__value + self.__excess(G, s)) > FLOATING_POINT_EPSILON):
            print("Excess at source = ",self.__excess(G, s))
            print("Max flow         = ",self.__value);
            return False

        if (abs(self.__value + self.__excess(G, s)) > FLOATING_POINT_EPSILON):
            print("Excess at sink   = ",self.__excess(G, t))
            print("Max flow         = ",self.__value);
            return False
        
        for v in range(G.V()):
            if (v == s or v == t):
                continue
            elif (abs(self.__excess(G, v)) > FLOATING_POINT_EPSILON):
                print("Net flow out of",v,"doesn't equal zero")
                return False
            
        return True

    # check optimality conditions
    def __check(self,G,s,t):
        # check that flow is feasible
        if (not self.__isFeasible(G, s, t)):
            print("Flow is infeasible")
            return false

        # check that s is on the source side of min cut and that t is not on source side
        if (not inCut(s)):
            print("source",s,"is not on source side of min cut")
            return false
        
        if (inCut(t)):
            print("sink",t,"is on source side of min cut")
            return false

        # check that value of min cut = value of max flow
        mincutValue = 0.0
        for v in range(G.V()):
            for e in G.adj(v):
                if ((v == e.From()) and
                    inCut(e.From()) and
                    not inCut(e.To())):
                    mincutValue += e.capacity()

        if (abs(mincutValue - self.__value) > FLOATING_POINT_EPSILON):
            print("Max flow value =",self.__value,", min cut value = ",mincutValue)
            return false
        
        return true
