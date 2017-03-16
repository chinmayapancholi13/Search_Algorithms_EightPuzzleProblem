#include <bits/stdc++.h>
#include <sys/time.h>

using namespace std;

const long long int num_start_states = 100;
const string goal_state("123456780");
const string sentinel_string("#");

typedef struct eight_puzzle_node
{
  eight_puzzle_node();
  eight_puzzle_node(string start_state_argument);
  string board_state;
  long long int g_value;
  long long int h_value;
  long long int f_value;
  bool operator<(const struct eight_puzzle_node& node1);
  bool operator>(const struct eight_puzzle_node& node1);
  bool check_is_goal_state();
  long long int compute_h_value(long long int heuristic);
} eight_puzzle_node;

// double val1=0;
// double val2=0;
// long long int cnt1=0;

set<string> visited_states;
long long int expanded_states_count;

double latest_start_time;
double latest_end_time;
double time_taken;

bool eight_puzzle_node::operator<(const struct eight_puzzle_node& node1)
{
  return (board_state < node1.board_state);
}

bool eight_puzzle_node::operator>(const struct eight_puzzle_node& node1)
{
  return (board_state > node1.board_state);
}

eight_puzzle_node::eight_puzzle_node()
{
  board_state = "";
  g_value = 0;
  h_value = 0;
  f_value = 0;
}

eight_puzzle_node::eight_puzzle_node(string start_state_argument)
{
  board_state = start_state_argument;
  g_value = 0;
  h_value = 0;
  f_value = 0;
}

bool eight_puzzle_node::check_is_goal_state()
{
  long long int i;
  if(this->board_state.compare(goal_state)==0)
  {
    // time_t rawtime1;
    // struct tm *timeinfo1;
    // time(&rawtime1);
    // timeinfo1 = localtime(&rawtime1);
    // latest_end_time = timeinfo1->tm_hour*3600 + timeinfo1->tm_min*60 + timeinfo1->tm_sec;
    timeval tim;
    gettimeofday(&tim, NULL);
    latest_end_time = tim.tv_sec+(tim.tv_usec/1000000.0);

    time_taken = (latest_end_time-latest_start_time);
    return true;
  }
  else
  {
    return false;
  }
}

long long int eight_puzzle_node::compute_h_value(long long int heuristic)
{
  if(heuristic == 1)
  {
    return 0;
  }
  else if(heuristic == 2)
  {
    long long int i, misplaced_tiles = 0;
    for(i=0;i<9;i++)
    {
      if(this->board_state[i]!=goal_state[i] && this->board_state[i]!='0')
      {
        misplaced_tiles++;
      }
    }
    return misplaced_tiles;
  }
  else if(heuristic == 3)
  {
    long long int i, distance_sums = 0, position_of_tile;
    for(i=1;i<=8;i++)
    {
      position_of_tile = (this->board_state).find_first_of('0'+i);
      distance_sums += (abs(position_of_tile+1-i));
    }
    return distance_sums;
  }
}

struct compare_function_min
{
  bool operator()(const eight_puzzle_node& node1, const eight_puzzle_node& node2) const
  {
     return (node1.f_value>=node2.f_value);
  }
};

priority_queue<eight_puzzle_node, vector<eight_puzzle_node>, compare_function_min> pq_nodes_min;

eight_puzzle_node do_left_shift(eight_puzzle_node& old_state, long long int heuristic)
{
  eight_puzzle_node new_state("#");
  long long int space_position;
  space_position = (old_state.board_state).find_first_of('0');
  if((space_position%3)!=0)
  {
    new_state.board_state = old_state.board_state;
    char c1;
    c1 = (new_state.board_state)[space_position-1];
    (new_state.board_state)[space_position] = c1;
    (new_state.board_state)[space_position-1] = '0';
    new_state.g_value = old_state.g_value + 1;
    new_state.h_value = old_state.compute_h_value(heuristic);
    new_state.f_value = new_state.g_value + new_state.h_value;
  }
  return new_state;
}

eight_puzzle_node do_right_shift(eight_puzzle_node& old_state, long long int heuristic)
{
  eight_puzzle_node new_state("#");
  long long int space_position;
  space_position = (old_state.board_state).find_first_of('0');
  if((space_position%3)!=2)
  {
    new_state.board_state = old_state.board_state;
    char c1;
    c1 = (new_state.board_state)[space_position+1];
    (new_state.board_state)[space_position] = c1;
    (new_state.board_state)[space_position+1] = '0';
    new_state.g_value = old_state.g_value + 1;
    new_state.h_value = old_state.compute_h_value(heuristic);
    new_state.f_value = new_state.g_value + new_state.h_value;
  }
  return new_state;
}

eight_puzzle_node do_top_shift(eight_puzzle_node& old_state, long long int heuristic)
{
  eight_puzzle_node new_state("#");
  long long int space_position;
  space_position = (old_state.board_state).find_first_of('0');
  if((space_position)>=3)
  {
    new_state.board_state = old_state.board_state;
    char c1;
    c1 = (new_state.board_state)[space_position-3];
    (new_state.board_state)[space_position] = c1;
    (new_state.board_state)[space_position-3] = '0';
    new_state.g_value = old_state.g_value + 1;
    new_state.h_value = old_state.compute_h_value(heuristic);
    new_state.f_value = new_state.g_value + new_state.h_value;
  }
  return new_state;
}

eight_puzzle_node do_bottom_shift(eight_puzzle_node& old_state, long long int heuristic)
{
  eight_puzzle_node new_state("#");
  long long int space_position;
  space_position = (old_state.board_state).find_first_of('0');
  if((space_position)<=5)
  {
    new_state.board_state = old_state.board_state;
    char c1;
    c1 = (new_state.board_state)[space_position+3];
    (new_state.board_state)[space_position] = c1;
    (new_state.board_state)[space_position+3] = '0';
    new_state.g_value = old_state.g_value + 1;
    new_state.h_value = old_state.compute_h_value(heuristic);
    new_state.f_value = new_state.g_value + new_state.h_value;
  }
  return new_state;
}

// bool implement_A_star_search(eight_puzzle_node& start_state_arg, long long int heuristic)
// {
//   timeval tim;
//   gettimeofday(&tim, NULL);
//   latest_start_time = tim.tv_sec+(tim.tv_usec/1000000.0);

//   expanded_states_count = 0;
//   visited_states.clear();
//   // visited_states.insert(start_state_arg.board_state);
//   pq_nodes_min = priority_queue<eight_puzzle_node, vector<eight_puzzle_node>, compare_function_min>() ;
//   pq_nodes_min.push(start_state_arg);
//   while(1)
//   {
//     if(pq_nodes_min.empty())
//     {
//       return false;
//     }

//     eight_puzzle_node current_search_state = pq_nodes_min.top();
//     pq_nodes_min.pop();

//     if(visited_states.find(current_search_state.board_state)!=visited_states.end())
//     {
//       continue;
//     }

//     expanded_states_count++;
//     visited_states.insert(current_search_state.board_state);

//     if(current_search_state.check_is_goal_state())
//     {
//       return true;
//     }

//     eight_puzzle_node left_str = do_left_shift(current_search_state, heuristic);
//     if((left_str.board_state).compare(sentinel_string)!=0 && visited_states.find(left_str.board_state)==visited_states.end())
//     {
//       // cout<<"here1";
//       // visited_states.insert(left_str.board_state);
//       pq_nodes_min.push(left_str);
//     }

//     eight_puzzle_node right_str = do_right_shift(current_search_state, heuristic);
//     if((right_str.board_state).compare(sentinel_string)!=0 && visited_states.find(right_str.board_state)==visited_states.end())
//     {
//       // cout<<"here2";
//       // visited_states.insert(right_str.board_state);
//       pq_nodes_min.push(right_str);
//     }

//     eight_puzzle_node top_str = do_top_shift(current_search_state, heuristic);
//     if((top_str.board_state).compare(sentinel_string)!=0 && visited_states.find(top_str.board_state)==visited_states.end())
//     {
//       // cout<<"here3";
//       // visited_states.insert(top_str.board_state);
//       pq_nodes_min.push(top_str);
//     }

//     eight_puzzle_node bottom_str = do_bottom_shift(current_search_state, heuristic);
//     if((bottom_str.board_state).compare(sentinel_string)!=0 && visited_states.find(bottom_str.board_state)==visited_states.end())
//     {
//       // cout<<"here4";
//       // visited_states.insert(bottom_str.board_state);
//       pq_nodes_min.push(bottom_str);
//     }
//   }
// }

long long int iterative_IDA_search(eight_puzzle_node node1, long long int limit1, long long int heuristic)
{
  long long int current_f_val = node1.f_value;
  if(current_f_val > limit1)
  {
    return current_f_val;
  }

  if(node1.check_is_goal_state())
  {
    expanded_states_count++;
    return (-1);
  }

  long long int min_value1 = (-1);

  eight_puzzle_node left_str = do_left_shift(node1, heuristic);
  if((left_str.board_state).compare(sentinel_string)!=0)
  {
    long long int succ_ret_val1 = iterative_IDA_search(left_str, limit1, heuristic);
    if(succ_ret_val1==(-1))
    {
      return (-1);
    }
    else if(succ_ret_val1==(-2))
    {
      return (-2);
    }
    else
    {
      if(min_value1 == (-1))
      {
        min_value1 = succ_ret_val1;
      }
      else if(succ_ret_val1<min_value1)
      {
        min_value1 = succ_ret_val1;
      }
    }
  }

  eight_puzzle_node right_str = do_right_shift(node1, heuristic);
  if((right_str.board_state).compare(sentinel_string)!=0)
  {
    long long int succ_ret_val1 = iterative_IDA_search(right_str, limit1, heuristic);
    if(succ_ret_val1==(-1))
    {
      return (-1);
    }
    else if(succ_ret_val1==(-2))
    {
      return (-2);
    }
    else
    {
      if(min_value1 == (-1))
      {
        min_value1 = succ_ret_val1;
      }
      else if(succ_ret_val1<min_value1)
      {
        min_value1 = succ_ret_val1;
      }
    }
  }

  eight_puzzle_node top_str = do_top_shift(node1, heuristic);
  if((top_str.board_state).compare(sentinel_string)!=0)
  {
    long long int succ_ret_val1 = iterative_IDA_search(top_str, limit1, heuristic);
    if(succ_ret_val1==(-1))
    {
      return (-1);
    }
    else if(succ_ret_val1==(-2))
    {
      return (-2);
    }
    else
    {
      if(min_value1 == (-1))
      {
        min_value1 = succ_ret_val1;
      }
      else if(succ_ret_val1<min_value1)
      {
        min_value1 = succ_ret_val1;
      }
    }
  }

  eight_puzzle_node bottom_str = do_bottom_shift(node1, heuristic);
  if((bottom_str.board_state).compare(sentinel_string)!=0)
  {
    long long int succ_ret_val1 = iterative_IDA_search(bottom_str, limit1, heuristic);
    if(succ_ret_val1==(-1))
    {
      return (-1);
    }
    else if(succ_ret_val1==(-2))
    {
      return (-2);
    }
    else
    {
      if(min_value1 == (-1))
      {
        min_value1 = succ_ret_val1;
      }
      else if(succ_ret_val1<min_value1)
      {
        min_value1 = succ_ret_val1;
      }
    }
  }

  expanded_states_count++;
  return min_value1;
}

bool implement_IDA_star_search(eight_puzzle_node& start_state_arg, long long int heuristic)
{
  timeval tim;
  gettimeofday(&tim, NULL);
  latest_start_time = tim.tv_sec+(tim.tv_usec/1000000.0);

  long long int depth_bound = start_state_arg.h_value;
  long long int bound_returned;
  expanded_states_count = 0;

  while(1)
  {
    if(depth_bound > 32)
    {
      return false;
    }
    bound_returned = iterative_IDA_search(start_state_arg, depth_bound, heuristic);
    if(bound_returned == (-1))
    {
      return true;
    }
    else if(bound_returned == (-2))
    {
      return false;
    }
    else
    {
      depth_bound = bound_returned;
    }
  }
}

// bool implement_search(eight_puzzle_node& start_state_arg, long long int heuristic_used, long long int algorithm_used)
// {
//   if(algorithm_used == 1)
//   {
//     return implement_A_star_search (start_state_arg, heuristic_used);
//   }
//   else if(algorithm_used == 2)
//   {
//     return implement_IDA_star_search (start_state_arg, heuristic_used);
//   }
// }

long long int my_random_function(long long int i)
{
  return (rand()%i);
}

string generate_random_start_state()
{
  vector<long long int> tiles_list;
  string random_string;
  long long int i;
  for(i=0;i<=8;i++)
  {
    tiles_list.push_back(i);
  }
  random_shuffle(tiles_list.begin(), tiles_list.end(),my_random_function);
  for(i=0;i<=8;i++)
  {
    random_string.push_back(tiles_list[i]+'0');
  }
  return random_string;
}

bool check_if_solvable(eight_puzzle_node& start_state_arg)
{
  long long int i,j;
  long long int space_position = (start_state_arg.board_state).find_first_of('0');
  long long int value2 = 0;

  for(i=0;i<9;i++)
  {
    if((start_state_arg.board_state)[i] == '0')
    {
      continue;
    }
    for(j=i+1;j<9;j++)
    {
      if((start_state_arg.board_state)[j]<(start_state_arg.board_state)[i] && (start_state_arg.board_state)[j]!='0')
      {
        value2++;
      }
    }
  }
  if(value2%2==0)
  {
    return true;
  }
  if(value2%2==1)
  {
    return false;
  }
}

int main()
{
  srand(unsigned(time(0)));
  long long int heuristic_used;   // 1, 2, 3
  long long int i,j,k;
  bool returned_value;

  for(i=1; i<=1; i++)
  // for(i=1; i<=num_start_states; i++)
  {
    string new_start_state_str = generate_random_start_state();   //"617502438"
    cout<<"\nTest case #"<<i<<" : Randomly generated start configuration of the board : "<<new_start_state_str<<"\n";
      for(heuristic_used=3; heuristic_used>=2; heuristic_used--)
      {
        if(heuristic_used == 1)
        {
          cout<<"\n\t\tHeuristic used : Heuristic value = 0 always\n";
        }
        else if(heuristic_used == 2)
        {
          cout<<"\n\t\tHeuristic used : Number of misplaced tiles count\n";
        }
        else if(heuristic_used == 3)
        {
          cout<<"\n\t\tHeuristic used : Manhattan Distance\n";
        }

        eight_puzzle_node new_start_state_node(new_start_state_str);
        new_start_state_node.h_value = new_start_state_node.compute_h_value(heuristic_used);
        new_start_state_node.f_value = new_start_state_node.h_value;

        if(!check_if_solvable(new_start_state_node))
        {
          cout<<"\t\tNo solution exists for the given starting configuration.\n";
          continue;
        }

        returned_value = implement_IDA_star_search(new_start_state_node, heuristic_used);
        if(returned_value)
        {
          cout<<"\t\tNumber of nodes expanded : "<<expanded_states_count<<"\n";
          cout<<"\t\tTime taken for finding solution : "<<time_taken<<"\n";
          // val1+=(expanded_states_count);
          // val2+=(time_taken);
          // cnt1++;
        }
        else
        {
          cout<<"\t\tNo solution exists\n";
        }
      }
  }
  // cout<<(val1/cnt1)<<"\t\t"<<(val2/cnt1)<<"\n";
  return 0;
}
