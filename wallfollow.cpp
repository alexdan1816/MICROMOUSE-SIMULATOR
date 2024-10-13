    #include <iostream>
    using namespace std;
    const int distance_threshold =10;

    int readfrontsensor()
    {
        return 0;
    }
    int readleftsensor()
    {
        return 0;
    }
    int readrightsensor()
    {
        return 0;
    }

    void motorSetSpeed(int leftmotorspeed, int rightmotorspeed)
    {

    }

    void wallfollowing()
    {
        int left=readleftsensor();
        int right=readrightsensor();
        int front=readfrontsensor();
        if(!left)
        {
            //turn left
        }
        else if(front && !right)
        {
            //turn right
        }
        else if(!front && right)
        {
            //go ahead
        }
        else 
        {
            //turn around
        }
    }

    int main()
    {
        return 0;
    }