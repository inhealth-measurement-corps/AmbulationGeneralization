
//A patient class to give recommendations to
//vector<vector<string>> patients; //each line of idata file

class Patient{       // The class
  private:  

    //ambulation parameters - from research paper pg. 7
    int id;
    double length_of_stay;
    double total_ambulations;
    double ambulation_frequency;
    double compliance_one_day;
    double compliance_two_day;
    double compliance_three_day;
    double mean_ambulation_distance;
    double longest_ambulation_distance;
    double shortest_ambulation_distance;
    double min_speed_ambulation;
    double max_speed_ambulation;
    double days_ambulation;
    double ambulation_frequency;


  public:

    Patient() {
      
    }

    Patient (int i, double los, double ta, double af, double cod, double ctwod, double cthreed, double mad, double lad, double sad, double minspeeda, double maxspeeda, double days_walk_param, double param_freq) {
        id = i;
        length_of_stay = los;
        total_ambulations = ta;
        ambulation_frequency = af;
        compliance_one_day = cod;
        compliance_two_day = ctwod;
        compliance_three_day = cthreed;
        mean_ambulation_distance = mad;
        longest_ambulation_distance = lad;
        shortest_ambulation_distance = sad;
        min_speed_ambulation = minspeeda;
        max_speed_ambulation = maxspeeda;
        days_ambulation = days_walk_param;
        ambulation_frequency = param_freq;
    }


    bool getRecommendation(Patient p) {
      if (p.ambulation_frequency < ambulationRecommendation(p.ambulation_frequency)) {
        //patient did not achieve recommendation goal
        return true;
      } 

      return false;
    }

  double ambulationRecommendation(double ambulation_frequency) {
    //blackbox that can give us recommendation
  }


    //getters and setters

     int getId(){
      return id;
    }

 
    void setId(int new_id)
    {
      id = new_id;
    }


    double getLength_of_stay(){
      return length_of_stay;
    }

 
    void setLength_of_stay(double new_length_of_stay){
      length_of_stay = new_length_of_stay;
    }


    double getTotal_ambulations(){
      return total_ambulations;
    }


    void setTotal_ambulations(double new_total_ambulations){
      total_ambulations = new_total_ambulations;
    }


    double getAmbulation_frequency(){
      return ambulation_frequency;
    }


    void setAmbulation_frequency(double new_ambulation_frequency){
      ambulation_frequency = new_ambulation_frequency;
    }


    double getCompliance_one_day(){
      return compliance_one_day;
    }

 
    void setCompliance_one_day(double new_compliance_one_day){
      compliance_one_day = new_compliance_one_day;
    }


    double getCompliance_two_day(){
      return compliance_two_day;
    }


    void setCompliance_two_day(double new_compliance_two_day){
      compliance_two_day = new_compliance_two_day;
    }

    double getCompliance_three_day(){
      return compliance_three_day;
    }


    void setCompliance_three_day(double new_compliance_three_day){
      compliance_three_day = new_compliance_three_day;
    }

    double getMean_ambulation_distance(){
      return mean_ambulation_distance;
    }

    void setMean_ambulation_distance(double new_mean_ambulation_distance){
      mean_ambulation_distance = new_mean_ambulation_distance;
    }

    double getLongest_ambulation_distance(){
      return longest_ambulation_distance;
    }

    void setLongest_ambulation_distance(double new_longest_ambulation_distance){
      longest_ambulation_distance = new_longest_ambulation_distance;
    }

    double getShortest_ambulation_distance(){
      return shortest_ambulation_distance;
    }

    void setShortest_ambulation_distance(double new_shortest_ambulation_distance){
      shortest_ambulation_distance = new_shortest_ambulation_distance;
    }

    double getMin_speed_ambulation(){
      return min_speed_ambulation;
    }

    void setMin_speed_ambulation(double new_min_speed_ambulation){
      min_speed_ambulation = new_min_speed_ambulation;
    }

    double getMax_speed_ambulation(){
      return max_speed_ambulation;
    }

    void setMax_speed_ambulation(double new_max_speed_ambulation){
      max_speed_ambulation = new_max_speed_ambulation;
    }

    double getDays_ambulation(){
      return days_ambulation;
    }

    void setDays_ambulation(double new_days_ambulation){
      days_ambulation = new_days_ambulation;
    }

    double getAmbulation_frequency(){
      return ambulation_frequency;
    }


    void setAmbulation_frequency(double new_ambulation_frequency){
      ambulation_frequency = new_ambulation_frequency;
    }
    



    





    
};