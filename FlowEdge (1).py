FLOATING_POINT_EPSILON = 1E-10

class FlowEdge:

    def __init__(self, v, w, cap):
        if (v < 0 or w < 0):
            raise Exception("vertex index must be a non-negative integer")
        if (cap < 0):
            raise Exception("Edge capacity must be non-negative")
        self.__v = v
        self.__w = w
        self.__capacity = cap
        self.__flow = 0.0
        
    
    @classmethod
    def fromFlow(class_object, v, w, cap, flow):
        obj = class_object(v,w,cap)
        if (v < 0 or w < 0):
            raise Exception("vertex index must be a non-negative integer")
        if (cap < 0):
            raise Exception("Edge capacity must be non-negative")
        if (not (flow >= 0.0)):
            raise Exception("Flow is negative")
        if (not (flow <= cap)):
            raise Exception("Flow exceeds capacity")
        obj.__flow = flow
        return obj
        
    
    def From(self):
        return self.__v

    def To(self):
        return self.__w

    def capacity(self):
        return self.__capacity

    def flow(self):
        return self.__flow

    def other(self, vertex):
        if (vertex == self.__v):
            return self.__w
        elif (vertex == self.__w):
            return self.__v
        else:
            raise Exception("Invalid endpoint")

    def residualCapacityTo(self, vertex):
        if (vertex == self.__v):
            return self.__flow
        elif (vertex == self.__w):
            return self.__capacity - self.__flow
        else:
            raise Exception("Invalid endpoint")

    def addResidualFlowTo(self, vertex, delta):
        if (not (delta >= 0.0)):
            raise Exception("Delta must be nonnegative")
        
        if (vertex == self.__v):
            self.__flow -= delta
        elif (vertex == self.__w):
            self.__flow += delta
        else:
            raise Exception("Invalid endpoint")
        
        if (abs(self.__flow) <= FLOATING_POINT_EPSILON):
            self.__flow = 0.0;
        if (abs(self.__flow - self.__capacity) <= FLOATING_POINT_EPSILON):
            self.__flow = self.__capacity;

        if (not (self.__flow >= 0.0)):
            raise Exception("Flow is negative")
        if (not (self.__flow <= self.__capacity)):
            raise Exception("Flow exceeds capacity")

    def __str__(self):
        return str(self.__v) + "->" +\
               str(self.__w) + " " +\
               str(self.__flow) + "/" +\
               str(self.__capacity);
