#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <time.h>

namespace constants
{
	const int MAX_LADDER = 8;
	const int MAX_SNAKE = 8;
	const int MAX_POSITION = 100;
	const int INITIAL = 0;
	const int FINAL = 1;
	const int OFFSET = 11;
}

cv::Mat goti1 = cv::imread("red_piece.png", CV_LOAD_IMAGE_COLOR);
cv::Mat goti2 = cv::imread("blue_piece.png", CV_LOAD_IMAGE_COLOR);
cv::Mat board = cv::imread("board.jpg", CV_LOAD_IMAGE_COLOR);
cv::Mat game=board;

const int ladder[constants::MAX_LADDER][2] = 
{{1, 38}, {4, 14},
 {9, 31}, {21, 42},
 {28, 84}, {51, 67}, 
 {71, 91}, {80, 100}};
 
const int snake[constants::MAX_SNAKE][2] =
{{17, 7}, {54, 34},
 {62, 19}, {64, 60},
 {87, 24}, {93, 73},
 {95, 75}, {98, 79}};

int roll_the_dice ()
{
	srand(time(NULL));
	int random = rand()%6+1;
	//std::cout<<random<<std::endl;
	return random;
}

void toggle_player (bool &player_index)
{
	player_index = !player_index;
}

bool stepped_on_ladder(int position, bool is_enabled)
{
	int ladder_index;
	using namespace constants;
	if(is_enabled)
	for(ladder_index = 0 ; ladder_index < MAX_LADDER ; ladder_index++)
	{
		if(ladder[ladder_index][INITIAL]==position)
		{
		std::cout<<"climbed ladder of Index"<<ladder_index<<std::endl;
		return true;
		}
	}
	
	return false;
}

int get_other_end_of_ladder (int position)
{
	int ladder_index;
	using namespace constants;
	for(ladder_index = 0 ; ladder_index < MAX_LADDER ; ladder_index++)
	{
		if(ladder[ladder_index][INITIAL]==position)
		return ladder[ladder_index][FINAL];
		
	}
}
	
bool bitten_by_snake(int position)
{
	int snake_index;
	using namespace constants;
	for(snake_index = 0 ; snake_index < MAX_SNAKE ; snake_index++)
	{
		if(snake[snake_index][INITIAL]==position)
		{
		std::cout<<"Bitten by snake of Index "<<snake_index<<std::endl;
		return true;
		}
	}
	
	return false;
}

int get_tail_of_snake (int position)
{
	int snake_index;
	using namespace constants;
	for(snake_index = 0 ; snake_index < constants::MAX_LADDER ; snake_index++)
	{
		if(snake[snake_index][INITIAL]==position)
		return snake[snake_index][FINAL];
	}
}

int get_x_coordinate(int position)
{
	int x_coordinate = (((position-1)/10)%2)?(board.rows-constants::OFFSET-87*((position-1)%10)-87):(constants::OFFSET+87*((position-1)%10));
	return x_coordinate;
}

int get_y_coordinate(int position)
{
	int y_coordinate = board.cols-constants::OFFSET-87*((position-1)/10);
	return y_coordinate;
}

void show_game(int x1, int y1, int x2, int y2)
{
	int row, col;
	using namespace cv;
	game=imread("board.jpg", CV_LOAD_IMAGE_COLOR);
	for(row=0;row<86;row++)
		for(col=0;col<86;col++)
		{
			Vec3b & temp = game.at<Vec3b>(y1-col, x1+row);
			Vec3b goti = goti1.at<Vec3b>(col, row);
			if(!(goti[1]>100&&goti[2]>100&&goti[0]>100))
			{
				temp[0]=goti[0];
				temp[1]=goti[1];
				temp[2]=goti[2];
			}
			
			Vec3b & temp_ = game.at<Vec3b>(y2-col, x2+row);
			Vec3b goti_ = goti2.at<Vec3b>(col, row);
			if(!(goti_[1]>100&&goti_[2]>100&&goti_[0]>100))
			{
				temp_[0]=goti_[0];
				temp_[1]=goti_[1];
				temp_[2]=goti_[2];
			}
		}
}

void move(int destination, bool player_index, int &initial, bool &is_enabled, int other_player_position)
{
	if(!is_enabled)
	{
		if((destination-initial)<6)
		{
			std::cout<<"Still trapped!"<<std::endl;
			return;
		}
		else
		{
			std::cout<<"set free!"<<std::endl;
			is_enabled = !is_enabled;
			destination-=7;
		}
	}
	
	{
		if(destination>100)
		{
			std::cout<<"Going beyond 100 is not allowed"<<std::endl;
			return;
		}
		
		if(!player_index)
		{
			show_game(get_x_coordinate(destination), get_y_coordinate(destination), 
			get_x_coordinate(other_player_position), get_y_coordinate(other_player_position));
		}
		else
		{
			show_game(get_x_coordinate(other_player_position), get_y_coordinate(other_player_position), 
			get_x_coordinate(destination), get_y_coordinate(destination));

		}
		initial=destination;
	}
}

void check_for_win(int position, bool &not_finished, bool player_index)
{
	if(position==100)
	{
		not_finished = false;
		std::cout<<((player_index)?"CPU won!":"You won!")<<std::endl;
	}
	return;
}

int main()
{
	bool not_finished=true, is_enabled[2]={false, false}, player_index=false;
	int step_count=0, position[2]={1, 1};
	cv::namedWindow("SnakeAndLadders", CV_WINDOW_NORMAL);
	imshow("SnakeAndLadders", game);
	
	show_game(get_x_coordinate(1), get_y_coordinate(1), get_x_coordinate(1), get_y_coordinate(1));
	
	
	while(not_finished)
	{
		step_count = roll_the_dice();
		if(step_count==6)
		{
			std::cout<<((player_index)?"CPU got ":"Yay! You got ")<<"6. "<<((player_index)?"CPU get ":"You get ")<<"another chance"<<std::endl;
			cv::waitKey(0);
			step_count += roll_the_dice();
			if(step_count==12)
			{
				std::cout<<((player_index)?"CPU got ":"Yay! You got ")<<"6 again. "<<((player_index)?"CPU get ":"You get ")<<"another chance" << std::endl;
				cv::waitKey(0);
				step_count +=roll_the_dice();
				if(step_count==18)
				{
					std::cout<<"Alas! Triple 6s"<<std::endl;
					toggle_player(player_index);
					cv::waitKey(0);
					continue;
				}
			}
		}
		std::cout<<((player_index)?"CPU got ":"You got ")<<step_count<<std::endl;
		
		move(step_count+position[player_index], player_index, position[player_index], is_enabled[player_index], position[!player_index]);
		if(stepped_on_ladder(position[player_index], is_enabled[player_index]))
		{
			move(get_other_end_of_ladder(position[player_index]), player_index, position[player_index], is_enabled[player_index], position[!player_index]);
		}
		
		if(bitten_by_snake(position[player_index]))
		{
			move(get_tail_of_snake(position[player_index]), player_index, position[player_index], is_enabled[player_index], position[!player_index]);
		}
		
		imshow("SnakeAndLadders", game);
		cv::waitKey(0);
		
		check_for_win(position[player_index], not_finished, player_index);
		toggle_player(player_index);
	}
}
