#include <bits/stdc++.h>
using namespace std;
class Order
{
public:
    int order_id;
    map<char,int> meals;
    float distance;
    Order(int id, map<char,int> m, float dist )
    {
        order_id=id;
        meals = m;
        distance = dist;
    }

    void Print()
    {

        cout<<order_id<<",";

        for(auto m: meals)
        {
            cout<<"["<<m.first<< "=>"<<m.second<<"]"<<",";
        }
         cout<<distance<<endl;

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
    bool operator()(const AdditionalTime& p1,const AdditionalTime& p2)
    {
        return p1.delivery_time >p2.delivery_time;
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
        if(curr.meals.find('A')!= curr.meals.end() && curr.meals.find('M')!= curr.meals.end())
        {
            cooking_slot+= curr.meals['A'] + 2*curr.meals['M'];
             if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
            cooking_time += main_course_cooking_time;
        }

        else if(curr.meals.find('A')!= curr.meals.end())
        {
            cooking_slot += curr.meals['A'];
               if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
             cooking_time += appetiser_cooking_time;

        }
        else
        {
             cooking_slot+= 2*curr.meals['M'];
                 if(cooking_slot>total_cooking_slot)
            {
                cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
                continue;
            }
             cooking_time+= main_course_cooking_time;


        }

        if(cooking_slot<=total_cooking_slot)
        {
            delivery_time += (cooking_time+ (curr.distance*dist_rest));
            ///No waiting required
            if(remaining_slot>=cooking_slot)
            {
                remaining_slot-= cooking_slot;
                if(delivery_time> 150)
                {
                    cout<<"Order "<<curr.order_id<<" is denied because the delivery time is too high "<<endl;
                        continue;
                }
                cout<<"Order "<<curr.order_id<<" will get delivered in "<<delivery_time<<" minutes"<<endl;
            }

         //   cout<<"Cooking time : " <<cooking_time<<endl;
         //   cout<<"Traveling time " <<(curr.distance*dist_rest)<<endl;
         ///Waiting Required - we will use heap to get mininum waiting time
            else
            {
                AdditionalTime _at = delivery_time_record.top();
                float waiting_time = _at.delivery_time;
                float available_slots = _at.cooking_slot;
               if(cooking_slot<=available_slots+remaining_slot)
                {

                    available_slots-= (cooking_slot-remaining_slot);
                    remaining_slot =0;
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
               //   cout<<"Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;

                delivery_time += waiting_time;
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
    string name;
    getline(cin,name,'\t');
    string one="";
    vector<string> str;
    map<char,int> mymap;
    int i=0;
    queue<Order> q;
   while(i<name.length())
    {
        if(name[i]=='[')
        {
            i++;
            while(name[i]!=']')
            {
                if(name[i]=='A' || name[i]=='M')
                {
                    mymap[name[i]]++;
                }
                i++;
            }
        }

        if(name[i]!=',' && name[i]!='\n' && name[i]!= ']')
        {
            one+= name[i];
        }
        if(name[i]=='\n')
        {
        stringstream ss;
        ss<<one; ///insert data to ss from one
        //cout<<one;
        int order_id;
        float distance;

        ss>>order_id>>distance; ///extract data in order till space from ss
        Order order(order_id,mymap,distance);
        q.push(order);
        mymap.clear();
        one = "";
        }
        i++;
    }

        Food_Delivery_Time_Calculator(q);
//        while(!q.empty())
//    {
//        Order curr = q.front();
//        q.pop();
//        cout<<curr.order_id<<",";
//
//        for(auto m: curr.meals)
//        {
//            cout<<"["<<m.first<< "=>"<<m.second<<"]"<<",";
//        }
//         cout<<curr.distance<<endl;
//    }



    return 0;
}
/*


Sample Input:
12, [A, A], 5
21, [A, M], 1
32, [M], 0.1
14, [M, M, M, M, A, A, A], 10
22, [A], 3


Sample Output:
Order 12 will get delivered in 57 minutes
Order 21 will get delivered in 37 minutes
Order 32 will get delivered in 29.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Order 22 will get delivered in 70.8 minutes
12,[A=>2],5
21,[A=>1],[M=>1],1
32,[M=>1],0.1
14,[A=>3],[M=>4],10
22,[A=>1],3



TC:1
12, [A, A, A, M, M], 5
21, [A, M], 1
32, [M], 0.1
14, [M, M, M, M, A, A, A], 10
22, [A, A, A], 3

Order 12 will get delivered in 69 minutes
Order 21 will get delivered in 106 minutes
Order 32 will get delivered in 98.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Order 22 will get delivered in 139.8 minutes


TC:2
12, [M, M, M], 5
21, [A, M], 1
32, [M], 0.1
14, [M, M, M, M, A, A, A], 10
22, [A, A, A], 3

Order 12 will get delivered in 69 minutes
Order 21 will get delivered in 106 minutes
Order 32 will get delivered in 98.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Order 22 will get delivered in 139.8 minutes


TC:4
12, [M, M, M, A, A], 5
21, [A, M], 1
32, [M], 0.1
14, [M, M, M, M, A, A, A], 10
22, [A, A, A], 3

Order 12 is denied because the restaurant cannot accommodate it
Order 21 will get delivered in 37 minutes
Order 32 will get delivered in 29.8 minutes
Order 14 is denied because the restaurant cannot accommodate it
Order 22 will get delivered in 78 minutes


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
