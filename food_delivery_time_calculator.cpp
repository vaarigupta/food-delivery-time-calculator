#include <bits/stdc++.h>
using namespace std;
///Object to represent the blueprint of the order having orderID , meals to be delivered and distance from restaurant
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
        cout<<"Order ID : "<<order_id<<"endl";

        for(auto m: meals)
        {
            cout<<"["<<m.first<< "=>"<<m.second<<"] ,";
        }
        cout<<endl<<"Distance from Restuarant : "<<distance<<endl;

    }

};

///Object to determine if waiting period required or not => keeps the record of delivery time and cooking slot required in every order
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

///Functor to compare the order objects based on their delivery time in the heap
struct CompareDeliveryTime
{
    bool operator()(const AdditionalTime& p1,const AdditionalTime& p2)
    {
        if(p1.delivery_time==p2.delivery_time)
        {
            return p1.cooking_slot< p2.cooking_slot;
        }
        return p1.delivery_time >p2.delivery_time;
    }
};
///Function to determine the delivery time for each orders present in the queue
void Food_Delivery_Time_Calculator(queue<Order> Orders)
{
    int total_cooking_slot = 7;
    int remaining_slot =7;
    float  appetiser_cooking_time = 17;
    float main_course_cooking_time = 29;
    float dist_rest = 8;
    float waiting_time;
    float available_slots;
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
            cooking_time += main_course_cooking_time;
        }

        else if(curr.meals.find('A')!= curr.meals.end())
        {
            cooking_slot += curr.meals['A'];
            cooking_time += appetiser_cooking_time;

        }
        else
        {
            cooking_slot+= 2*curr.meals['M'];
            cooking_time+= main_course_cooking_time;


        }
      //  cout<<"Cooking time : " <<cooking_time<<endl;
       // cout<<"Traveling time " <<(curr.distance*dist_rest)<<endl;
        if(cooking_slot>total_cooking_slot)
        {
            cout<<"Order "<<curr.order_id<<" is denied because the restaurant cannot accommodate it "<<endl;
            continue;
        }

        else
        {
            delivery_time += (cooking_time+ (curr.distance*dist_rest));
            ///No waiting required because there are available slots
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


            ///Waiting Required - we will use heap to get mininum waiting time
            else
            {
                AdditionalTime _at = delivery_time_record.top();
                waiting_time = _at.delivery_time;
                available_slots = _at.cooking_slot;
               //  cout<<"First time :"<<" Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;
                 if(cooking_slot<=available_slots+remaining_slot)
                {
                    available_slots-= (cooking_slot-remaining_slot);
                   //  cout<<"If condition :"<<" Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;
                    remaining_slot =0;
                    delivery_time_record.pop();
                    AdditionalTime temp(available_slots, waiting_time);
                    delivery_time_record.push(temp);
                }
                else
                {
                    while(cooking_slot>available_slots+remaining_slot)
                    {
                    delivery_time_record.pop();
                    _at = delivery_time_record.top();

                    if(waiting_time==_at.delivery_time)
                    {
                        while(!delivery_time_record.empty() && waiting_time==_at.delivery_time)
                        {
                        waiting_time = _at.delivery_time;
                         available_slots += _at.cooking_slot;
                         delivery_time_record.pop();
                         _at = delivery_time_record.top();

                        }
                    }
                    waiting_time = _at.delivery_time;
                    available_slots += _at.cooking_slot;
                  //  cout<<"else condition :"<<" Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;

                    }

                }
              // cout<<"Final Print : Waiting time :"<<waiting_time<<" available slot "<<available_slots<<endl;

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
    string integral_vals="";
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
            integral_vals+= name[i];
        }
        if(name[i]=='\n')
        {
            stringstream ss;
            ss<<integral_vals; ///insert data to ss from one
            int order_id;
            float distance;

            ss>>order_id>>distance; ///extract data in order till space from ss
            Order order(order_id,mymap,distance);
            q.push(order);
            mymap.clear();
            integral_vals = "";
        }
        i++;
    }

    Food_Delivery_Time_Calculator(q);
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

*/
