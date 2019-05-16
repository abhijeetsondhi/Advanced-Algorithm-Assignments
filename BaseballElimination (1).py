from FordFulkerson import FordFulkerson
from FlowNetwork import FlowNetwork
from FlowEdge import FlowEdge
class BaseballElimination:

    def __init__(self,filename):
        self.data = []
        self.teamNames = [];
        self.resultArr = [];
        self.result = False
        self.finalResult = []
        self.teams1Send = {}
        self.teams2Send = {}
        self.reference = 0
        self.flag = False
        txtFile = open(filename,"r")
        with txtFile as r:
            while 1:
                readings = r.readline()
                if not readings:
                    break
                else:
                    tempData = str(readings).split(' ')
                    self.teamNames.append(tempData[0])

        txtFile = open(filename,"r")
        with txtFile as r:
            while 1:
                readings = r.readline()
                if not readings:
                    break
                else:
                    tempData = str(readings).split(' ')
                    tempDict = {}
                    tempDict['teamName'] = tempData[0]
                    tempDict['teamWin'] = tempData[1]
                    tempDict['teamLose'] = tempData[2]
                    tempDict['teamLeft'] = tempData[3]
                    t = 3
                    for x in self.teamNames:
                        t = t+1
                        tempDict[x] = tempData[t]
                    self.data.append(tempDict)

    def numberOfTeams(self):
        return self.teamNames.count()

    def teams(self):
        return self.teamNames

    def wins(self, team):
        for x in self.data:
            if(x['teamName'] == team):
                return x['teamWin']


    def losses(self, team):
        for x in self.data:
            if(x['teamName'] == team):
                return x['teamLose']

    def remaining(self, team):
        for x in self.data:
            if(x['teamName'] == team):
                return x['teamLeft']

    def against(self, team1, team2):
        for x in self.data:
            if(x['teamName'] == team1):
                return x[team2]

    def isEliminated(self, team):
        table1 = {}
        table2 = {}
        val = 1
        count1 = 0
        count2 = 0
        while count1 < len(self.data)-1:
            count2 = count1 + 1
            while count2 < len(self.data):
                table1[str(count1) + "-" + str(count2)] = val
                val = val+1
                count2 = count2 + 1
            count1 = count1 + 1
        
        val2 = val
        self.reference = val2
        count1 = 0
        while count1 < len(self.data):
            table2[count1] = val
            val = val + 1
            count1 = count1 + 1
        G = FlowNetwork(1 + len(table1) + len(table2) + 1)
        for x in table1:
            t = str(x).split('-')
            G.addEdge(FlowEdge(0,int(table1[x]),int(self.against(self.data[int(t[0])]["teamName"] , self.data[int(t[1])]["teamName"]))))
            G.addEdge(FlowEdge(int(table1[x]),int(table2[int(t[0])]),float("inf")))
            G.addEdge(FlowEdge(int(table1[x]),int(table2[int(t[1])]),float("inf")))

        #
        # print(val)
        self.flag = False
        for x in table2:
            q = int(self.wins(team))
            w = int(self.remaining(team))
            e = int(self.wins(self.data[int(x)]["teamName"]))
            if(q+w < e):
                self.flag = True
                self.finalResult.append(self.data[int(x)]["teamName"])
                # G.addEdge(FlowEdge(int(table2[x]),val,0))
            elif not self.flag:
                G.addEdge(FlowEdge(int(table2[x]),val,int(q+w-e)))
        

        s = 0
        t = G.V()-1

      
        if not self.flag:
            maxflow = FordFulkerson(G, s, t)
            # print("Max flow from",s,"to",t)



            # print min-cut
            cut = []
            for v in range(G.V()):
                if (maxflow.inCut(v)):
                    cut.append(v)
            # print("Min cut:",cut)
            # print("Max flow value =",maxflow.value())
            self.teams1Send = table1
            self.teams2Send = table2
            if len(cut) == 1:
                self.result = False
                return False
            else:
                self.result = True
                self.resultArr = cut
                return True
        return True


    def certificateOfElimination(self, team):
        self.isEliminated(team)
        key = self.teams2Send.keys()
        listOfTeams = []
        startArr = 0
        if self.flag:
            return "{} is eliminated by subset {}".format(team,self.finalResult)
        if self.result == True:
            for x in self.resultArr:
                if(x >= self.reference):
                    startArr = self.resultArr.index(x)
                    break
            
            while(startArr < len(self.resultArr)):
                listOfTeams.append(self.data[list(self.teams2Send.keys())[list(self.teams2Send.values()).index(self.resultArr[startArr])]]["teamName"])
                startArr = startArr+1
            
            return "{} is eliminated by subset {}".format(team,listOfTeams)
        else:
            return "{} is not eliminated".format(team)
                




