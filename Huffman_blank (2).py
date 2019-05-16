
import heapq
import math
import sys

class Node:

    def __init__(self, letter, weight):

        self.letter = letter
        self.zero = None
        self.one = None
        self.weight = weight

    def __ne__(self, other):
        return not (self == other)

    def __lt__(self, other):
        return (self.weight < other.weight)
    
    def __le__(self, other):
        return (self.weight <= other.weight)
    

    def __repr__(self):
        return "Letter:'%s' Weight: %s" % (self.letter, self.weight)

class Huffman:
    def __init__(self):
        self.lookup = {}
        self.heap = []
        self.lookup2={}

        # Any other variables you would like to add...

    def __lt__(self, other):
        return (self.weight < other.weight)

    def decode(self,inFileName,outFileName):
        """
        inFileName is the uncompressed text file
        outFileName is the resulting compressed file
        """
        txtFile = open(inFileName,"r")
        outFile = open(outFileName,"wb")
        encoded_str = str()
        decoded = str()
        with txtFile as r:
            while 1:
                reading = r.readline()
                if not reading:
                    break
                else:
                    reading = str(reading).split('-')
                    if len(reading) == 2:
                        self.lookup[reading[0]] = reading[1]
                    else:
                        encoded_str = str(reading[0])
        
        for x in self.lookup:
            y = Node(x,int(self.lookup.get(x)))
            self.heap.append(y)
        
        heapq.heapify(self.heap)
        self.heap.sort()

        while len(self.heap) > 1:
            t = heapq.heappop(self.heap)
            u = heapq.heappop(self.heap)
            v = Node("",int(t.weight) + int(u.weight))
            if t <= u:
                v.zero = t
                v.one = u
            else:
                v.zero = u
                v.one = t
            heapq.heappush(self.heap,v)
        
        root = heapq.heappop(self.heap)
        self.printNodes(root,"")

        length = 0
        lengthMin = 5000
        for x in self.lookup2:
            if len(str(self.lookup2.get(x))) > length:
                length = len(str(self.lookup2.get(x)))
        
        for x in self.lookup2:
            if len(str(self.lookup2.get(x))) < lengthMin:
                lengthMin = len(str(self.lookup2.get(x)))
        
        start = 0
        end = length
        while start < len(encoded_str):
            while int(end - start) >= lengthMin:
                temp = str(encoded_str[start:end])
                if temp in list(self.lookup2.values()):
                    decoded = str(decoded) + str(list(self.lookup2.keys())[list(self.lookup2.values()).index(temp)])
                    start = end
                    end = start + length
                    if end >= len(encoded_str):
                        end = len(encoded_str)
                    break
                else:
                    end = end - 1
            if int(end - start) < lengthMin:
                start = end
                end = start + length
                if end > len(encoded_str):
                        end = len(encoded_str) 
        
        print(decoded)
        for x in decoded:
            outFile.write(x.encode("utf-8"))
        
    def encode(self,inFileName,outFileName):
        """
        inFileName is the compressed text file
        outFileName is the resulting uncompressed file
        """
        txtFile = open(inFileName,"rb")
        outFile = open(outFileName,"w")
        with txtFile as b:
            while 1:
                reading = b.read(1).decode("utf-8")
                if not reading:
                    break
                elif reading not in self.lookup:
                    self.lookup[reading] = 1
                else:
                    x = int(self.lookup.get(reading))
                    x = int(x) + int(1)
                    self.lookup[reading] = x
        
        txtFile.close()

        for t in self.lookup:
            w = Node(t,self.lookup.get(t))
            self.heap.append(w)
        
        heapq.heapify(self.heap)
        self.heap.sort()

        while len(self.heap) > 1:
            t = heapq.heappop(self.heap)
            u = heapq.heappop(self.heap)
            v = Node("",int(t.weight) + int(u.weight))
            if t <= u:
                v.zero = t
                v.one = u
            else:
                v.zero = u
                v.one = t
            heapq.heappush(self.heap,v)
        root = heapq.heappop(self.heap)
        self.printNodes(root,"")

        for x in self.lookup:
            outFile.write(x + "-" + str(self.lookup.get(x)))
            outFile.write("\n")

        txtFile = open(inFileName,"rb")


        with txtFile as b:
            while 1:
                reading = b.read(1).decode("utf-8")
                if not reading:
                    break
                else:
                    outFile.write("%s" % self.lookup2.get(reading))
        txtFile.close()
        outFile.close()


    def printNodes(self,root,s):
        if root == None:
            return
        else:
            if root.zero == None and root.one == None:
                self.lookup2[root.letter] = s
            self.printNodes(root.zero,s+'0')
            self.printNodes(root.one,s+'1')
            
    def printDict(self):
        for x in self.lookup2:
            print(x)
            print(self.lookup2.get(x))

class MainClass:
    if __name__ == "__main__":
        huf = Huffman()
        huf.encode("file.txt","out.txt")
        hufm = Huffman()
        hufm.decode("out.txt","out2.txt")



        
