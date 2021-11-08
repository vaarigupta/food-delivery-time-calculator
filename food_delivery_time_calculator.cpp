#include <bits/stdc++.h>
using namespace std;
class Order
{
public:
    int order_id;
    map<string,int> meals;
    float distance;
    Order(int id, map<string,int> m, float dist )
    {
        order_id=id;
        meals = m;
        distance = dist;
    }

};
class AdditionalTime
{

public:
    int cooking_slot;
    float delivery_time;

    AdditionalTime(int slot, float time)
    : cooking_slot(slot), delivery_time(time)
    {
    }

};

struct CompareDeliveryTime {
    bool operator()(AdditionalTime const& p1, AdditionalTime const& p2)
    {
        return p1.delivery_time > p2.delivery_time;
    }
};
void Food_Delivery_Time_Calculator(queue<Order> Orders)
{
    int total_cooking_slot = 7;
     int remaining_slot =7;
    float  appetiser_cooking_time = 17;
    float main_course_cooking_time = 29;
   float dist_rest = 8;
   priority_queue<AdditionalTime, vector<AdditionalTime>,CompareDeliveryTime > delivery_time_record;
    while(!Orders.empty())
    {
        Order curr = Orders.front();
        Orders.pop();
        float delivery_time =0;
        float cooking_time =0;
        int cooking_slot=0;

        bool appetiser_present = false;
        if(curr.meals.find("A")!= curr.meals.end() && curr.meals.find("M")!= curr.meals.end())
        {
            cooking_slot+= curr.meals["A"] + 2*curr.meals["M"];
             if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
            cooking_time += main_course_cooking_time;
        }

        else if(curr.meals.find("A")!= curr.meals.end())
        {
            cooking_slot += curr.meals["A"];
               if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
             cooking_time += appetiser_cooking_time;

        }
        else
        {
             cooking_slot+= 2*curr.meals["M"];
                 if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
             cooking_time+= main_course_cooking_time;


        }

        if(cooking_slot<=total_cooking_slot)
        {
            remaining_slot-= cooking_slot;
            cout<<"Cooking time : " <<cooking_time<<endl;
            delivery_time += (cooking_time+ (curr.distance*dist_rest));
            cout<<"Traveling time " <<(curr.distance*dist_rest)<<endl;
            if(remaining_slot<0)
            {
                AdditionalTime _at = delivery_time_record.top();
                float waiting_time = _at.delivery_time;
                float available_slots = _at.cooking_slot;
               if(cooking_slot<=available_slots)
                {
                    available_slots-= cooking_slot;
                    delivery_time_record.pop();
                    AdditionalTime temp(available_slots, waiting_time);
                    delivery_time_record.push(temp);
                }
                else
                {
                    delivery_time_record.pop();
                    _at = delivery_time_record.top();
                    waiting_time = _at.delivery_time;
                    available_slots = _at.cooking_slot;
                }
                  cout<<"Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;

                delivery_time += waiting_time;
                if(delivery_time> 150)
                {
                    cout<<"Order "<<curr.order_id<<" is denied because the delivery time is too high "<<endl;
                    continue;
                }

                cout<<"Order "<<curr.order_id<<" will get delivered in "<<delivery_time<<" minutes"<<endl;

            }
            else
            {
                if(delivery_time> 150)
                {
                    cout<<"Order "<<curr.order_id<<" is denied because the delivery time is too high "<<endl;
                        continue;
                }
                cout<<"Order "<<curr.order_id<<" will get delivered in "<<delivery_time<<" minutes"<<endl;
            }
            AdditionalTime _additional_time(cooking_slot, delivery_time);
             delivery_time_record.push(_additional_time);

        }

    }



}
int main()
{

    map<string, int> temp1;
    temp1["A"] =2;
    Order order1(12, temp1,5);
    temp1["A"] = 1;
    temp1["M"] = 1;
    Order order2(21, temp1,1);
    temp1.erase("A");
    temp1["M"] = 1;
    Order order3(32, temp1,0.1);
    temp1["A"] = 3;
    temp1["M"] = 4;
    Order order4(14, temp1,10);
    temp1.erase("M");
    temp1["A"] =1;
    Order order5(22, temp1,3);
    queue<Order> q;
    q.push(order1);
     q.push(order2);
      q.push(order3);
       q.push(order4);
        q.push(order5);


    Food_Delivery_Time_Calculator(q);

        while(!q.empty())
    {
        Order curr = q.front();
        q.pop();
        cout<<curr.order_id<<",";

        for(auto m: curr.meals)
        {
            cout<<"["<<m.first<< "=>"<<m.second<<"]"<<",";
        }
         cout<<curr.distance<<endl;
    }



    return 0;
}
/*

TC:1
Cooking time : 17
Order 12 will get delivered in 57 minutes
Cooking time : 29
Order 21 will get delivered in 37 minutes
Cooking time : 29
Order 32 will get delivered in 29.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Cooking time : 17
Order 22 will get delivered in 70.8 minutes
12,[A=>2],5
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>3],3


TC:2
Cooking time : 29
Order 12 will get delivered in 69 minutes
Cooking time : 29
Order 21 will get delivered in 106 minutes
Cooking time : 29
Order 32 will get delivered in 135.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Cooking time : 17
Order 22 is denied because the delivery time is too high
12,[A=>3],[M=>2],5
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>3],3


TC:3
Cooking time : 29
Order 12 will get delivered in 109 minutes
Cooking time : 29
Order 21 will get delivered in 146 minutes
Cooking time : 29
Order 32 is denied because the delivery time is too high
Order 14 is denied because the restaurant cannot accommodate it
Cooking time : 17
Order 22 is denied because the delivery time is too high
12,[M=>3],10
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>3],3


TC:4
Order 12 is denied because the restaurant cannot accommodate it
Cooking time : 29
Order 21 will get delivered in 37 minutes
Cooking time : 29
Order 32 will get delivered in 29.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Cooking time : 17
Order 22 will get delivered in 70.8 minutes
12,[A=>2],[M=>3],5
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>3],3



TC:5
Cooking time : 29
Traveling time 40
Order 12 will get delivered in 69 minutes
Cooking time : 29
Traveling time 8
Waiting time :69 available slot 4
Order 21 will get delivered in 106 minutes
Cooking time : 29
Traveling time 0.8
Waiting time :69 available slot 2
Order 32 will get delivered in 98.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Cooking time : 17
Traveling time 24
Waiting time :98.8 available slot 2
Order 22 will get delivered in 139.8 minutes
12,[A=>3],[M=>2],5
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>3],3


*/
