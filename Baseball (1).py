from BaseballElimination import BaseballElimination

def main():
    division = BaseballElimination("teams4.txt")

    #print(division.data);  
    for x in division.teamNames:
        print(division.certificateOfElimination(x))
    #print(division.against("Atlanta","New_York"))

        

        
    
            
if __name__ == "__main__":
    main()

