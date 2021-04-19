
//A patient class to give recommendations to

class Patient{       // The class
  public:  
    int id;
    double distance;
    double avg_speed;
    int total_ambulation;
    int goal_placeholder;
    Patient(int identification, double d, double speed, int ambulation, int goal) {
        id = identification;
        distance = d;
        avg_speed = speed;
        total_ambulation = ambulation;
        goal_placeholder = goal;

    }

          // Access specifier
    
};