#ifndef DISP_HPP
#define DISP_HPP

/**
 * @brief JUDAH - Jacob is equipped with a text-based user interface
 *
 * @file disp.hpp
 * @author  Norbert Bátfai <nbatfai@gmail.com>
 * @version 0.0.1
 *
 * @section LICENSE
 *
 * Copyright (C) 2015 Norbert Bátfai, batfai.norbert@inf.unideb.hu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * JACOB, https://github.com/nbatfai/jacob
 *
 * "The son of Isaac is Jacob." The project called Jacob is an experiment
 * to replace Isaac's (GUI based) visual imagination with a character console.
 *
 * ISAAC, https://github.com/nbatfai/isaac
 *
 * "The son of Samu is Isaac." The project called Isaac is a case study
 * of using deep Q learning with neural networks for predicting the next
 * sentence of a conversation.
 *
 * SAMU, https://github.com/nbatfai/samu
 *
 * The main purpose of this project is to allow the evaluation and
 * verification of the results of the paper entitled "A disembodied
 * developmental robotic agent called Samu Bátfai". It is our hope
 * that Samu will be the ancestor of developmental robotics chatter
 * bots that will be able to chat in natural language like humans do.
 *
 */

#include <cstring>
#include <sstream>

#include <fstream>

#include <ncurses.h>
#include <mutex>

using namespace std;

class Disp
{

int speed=0, speed2=0;
int out_counter=1, out_row_counter=0, out_scounter=1;
int in_counter=1, in_row_counter=0, in_scounter=1;
int row_counter_samufile=0;
fstream fs,read,sread;
string buffer,reader,r_content,out_content, sreader, s_content;

public:

  Disp()
  {
    initscr();

    cbreak();
    noecho();
    timeout ( 0 );
    curs_set ( FALSE );

    clear();

    int  max_x, max_y;
    getmaxyx ( stdscr, max_y, max_x );

    mx = 0;
    my = 0;

    /*vi_w = newwin ( 10+2, max_x, 0, 0 );
    vi_iw = newwin ( 10+2-1, max_x, 1, 0 );*/
    
    vi_w = newwin ( 10+2, max_x/2, 0, 0 );
    vi_iw = newwin ( 10+2-1, max_x/2, 1, 0 );
    
    vi_w_out = newwin ( 10+2, max_x/2, 0, max_x/2 );
    vi_iw_out = newwin ( 10+2-1, max_x/2, 1, max_x/2 );

    log_w = newwin ( max_y- ( 10+2 ) - 3, max_x/2, 10+2, 0 );
    log_iw = newwin ( max_y- ( 10+2 ) - 3 -1, max_x/2, 10+2+1, 0 );
    
    slow_log_w = newwin ( max_y- ( 10+2 ) - 3, max_x/2, 10+2, max_x/2 );
    slow_log_iw = newwin ( max_y- ( 10+2 ) - 3 -1, max_x/2, 10+2+1, max_x/2 );

    shell_w = newwin ( 3, max_x, 10+2+max_y- ( 10+2 ) - 3, 0 );
    shell_iw = newwin ( 3-1, max_x, 10+2+max_y- ( 10+2 ) - 3-1, 0 );//batfai iroasos cucc

    
   
   

    start_color();
    
    init_pair ( 1, COLOR_BLACK, COLOR_WHITE );
    init_pair ( 2, COLOR_WHITE, COLOR_RED );
    init_pair ( 3, COLOR_WHITE, COLOR_BLUE );
    init_pair ( 4, COLOR_WHITE, COLOR_GREEN );
    init_pair ( 5, COLOR_WHITE, COLOR_CYAN );
    init_pair ( 6, COLOR_WHITE, COLOR_YELLOW );


    wbkgd ( vi_w, COLOR_PAIR ( 2 ) );
    wbkgd ( vi_iw, COLOR_PAIR ( 1 ));
    
    wbkgd ( vi_w_out, COLOR_PAIR ( 3 ) );
    wbkgd ( vi_iw_out, COLOR_PAIR ( 1 ));

    wbkgd ( log_w, COLOR_PAIR ( 6 ) );
    wbkgd ( log_iw, COLOR_PAIR ( 1 ) );

    wbkgd ( shell_w, COLOR_PAIR ( 5 ) );
    wbkgd ( shell_iw, COLOR_PAIR ( 1 ) );

    wbkgd ( slow_log_w, COLOR_PAIR ( 4 ) );
    wbkgd ( slow_log_iw, COLOR_PAIR ( 1 ) );

    nodelay ( shell_iw, TRUE );
    keypad ( shell_iw, TRUE );
    scrollok ( log_iw, TRUE );

    scrollok ( slow_log_iw, TRUE );
    scrollok ( vi_iw_out, TRUE );


    ui( );

  }

  ~Disp()
  {
    delwin ( vi_w );
    delwin ( vi_iw );
    
    delwin ( vi_w_out );
    delwin ( vi_iw_out );

    delwin ( log_w );
    delwin ( log_iw );

    delwin ( shell_w );
    delwin ( shell_iw );

    delwin ( slow_log_w );
    delwin ( slow_log_iw );
    endwin();
  }

  void shell ( std::string msg )
  {
    ncurses_mutex.lock();
    ui();
    werase ( shell_iw );
    mvwprintw ( shell_w, 0, 1, " Caregiver shell " );
    mvwprintw ( shell_iw, 0, 1, "Norbi> " );
    waddstr ( shell_iw, msg.c_str() );
    wrefresh ( shell_iw );
    ncurses_mutex.unlock();
  }

  void vi ( std::string msg )
  {
	   
    if ( ncurses_mutex.try_lock() )
      {
        ui();
        werase ( vi_iw );
        wmove ( vi_iw, 0, 0 );
        waddstr ( vi_iw, msg.c_str() );
        
        mvwprintw ( vi_w, 0, 0, " Amminadab's visual imagery " );
        wrefresh ( vi_iw );
        ncurses_mutex.unlock();
      
      
	    	   
	    	
	    
      } 	
  }

  void vi ( char* vi_console )//felso resz
  {
    if ( ncurses_mutex.try_lock() )
      {
        ui();
        werase ( vi_iw );

        for ( int i {0}; i<10; ++i )
          {
            wmove ( vi_iw, i+1, 1 );
            for ( int j {0}; j<80; ++j )
              {
                char c = vi_console[i*80+j];

                if ( isprint ( c ) )
                  {
                    if ( isdigit ( c ) )
                      {
                        wattron ( vi_iw,COLOR_PAIR ( c-'0'+2 ) );
                        waddch ( vi_iw, c );
                        wattroff ( vi_iw,COLOR_PAIR ( c-'0'+2 ) );
                      }
                    else
                      waddch ( vi_iw, c );

                  }
              }
          }

        mvwprintw ( vi_w, 0, 0, " Amminadab's visual imagery " );
        wrefresh ( vi_iw );
        ncurses_mutex.unlock();
	
	
      }
    
      
          
	  
      
  }
  


  void log ( std::string msg ) //bal also resz
  {
    ncurses_mutex.lock();
    ui();

    msg =  msg + "\n";
    
    std:stringstream ss;
    stringstream samuf;

 
    
    if(row_counter_samufile<1000)
	    {
	    	if(row_counter_samufile==0)
	    	{
	    	   
	
	    	    samuf << in_scounter;
	    	    sreader = "kifi";
	    
	    	    sread.open(sreader.c_str(), ios_base::in);
		
	    	    row_counter_samufile++;
	      	}
	    	if(speed2==50)//jobb also
	    	{
		    getline(sread,s_content);		

	    	    row_counter_samufile++;
		    speed2=0;
		
		    s_content=s_content + "\n"; 

		    waddstr ( vi_iw_out, s_content.c_str());


		        		    wrefresh ( vi_iw_out );

	    	}
	    	speed2++;
	    
	    }

	    else if(row_counter_samufile==1000)
	    {
	      sread.close();
	    
	    	//samuf.str("");
	    	//samuf.clear();
	
	    	//in_counter++;

	    	samuf << in_scounter-1;
	    	sreader = samuf.str();
	    
	    	

	      remove(sreader.c_str());

	    	row_counter_samufile=0;
	    }

  
    if(msg.compare("Saving Samu...\n")==0)  
    {	
        for(speed=0;speed<=out_counter;speed++)
 	{
            ss.str("");                   
	    ss.clear();

	    ss << speed;
            buffer = ss.str() + ".txt";

            remove(buffer.c_str());
	}
    }
    else
    {   
    	if(out_row_counter<1000)
    	{
	    if(out_row_counter==0)
	    {
	    	ss.str("");                   
	    	ss.clear();

	    	ss << out_counter;
            	buffer = ss.str() + ".txt";

            	fs.open(buffer.c_str(), fstream::out);
            }
	    
	    fs << msg;
 	    out_row_counter++;
    	}
    	else if(out_row_counter==1000)
    	{
    	    fs.close();
	    out_counter++;
	    out_row_counter=0;
    	}

    	if(out_counter>1)
    	{
	    if(in_row_counter<1000)
	    {
	    	if(in_row_counter==0)
	    	{
	    	    ss.str("");
	    	    ss.clear();
	
	    	    ss << in_counter;
	    	    reader = ss.str() + ".txt";
	    
	    	    read.open(reader.c_str(), ios_base::in);
		
	    	    in_row_counter++;
	      	}
	    	if(speed==200)
	    	{
		    getline(read,r_content);		

	    	    in_row_counter++;
		    speed=0;
		
		    r_content=r_content + "\n"; 

		    waddstr ( slow_log_iw, r_content.c_str());
    		    mvwprintw ( slow_log_w, 0, 0, " Amminadab's slow answers " );
    		    wrefresh ( slow_log_iw );
	    	}
	    	speed++;	
	    }
	    else if(in_row_counter==1000)
	    {
	     	read.close();
	    
	    	ss.str("");
	    	ss.clear();
	
	    	in_counter++;

	    	ss << in_counter-1;
	    	reader = ss.str() + ".txt";
	    
	    	remove(reader.c_str());
		

	    	in_row_counter=0;
	    }
    	}
    }	
	
    
    waddstr (log_iw, msg.c_str() );
    mvwprintw ( log_w, 0, 0, " Amminadab's answers " );
    wrefresh ( log_iw );

    ncurses_mutex.unlock();
  }

  void cg_read()
  {
    int ch;
    if ( ( ch = wgetch ( shell_iw ) ) != ERR )
      {

        if ( ch == '\n' )
          {
            std::string ret ( buf );
            buf.clear();
            shell ( buf );
            throw ret;
          }
        else if ( ch == KEY_BACKSPACE )
          {
            if ( buf.size() >= 1 )
              {
                buf.pop_back();
                shell ( buf );
              }
          }
        else
          {
            if ( isalnum ( ch ) || isspace ( ch ) )
              {
                if ( buf.length() < 78 )
                  {
                    buf += ch;
                    shell ( buf );
                  }
              }
          }
      }
  }

private:

  void ui ( void )
  {
    int  max_x, max_y;

    getmaxyx ( stdscr, max_y, max_x );

    if ( mx != max_x || my != max_y )
      {
        mx = max_x;
        my = max_y;
	
	wresize ( vi_w, 10+2, mx/2 );
        mvwin ( vi_w, 0, 0 );
        werase ( vi_w );

        wresize ( vi_iw, 10+2-1, mx/2 );
        mvwin ( vi_iw, 1, 0 );
        werase ( vi_iw );
	
	wresize ( vi_w_out, 10+2, mx/2 );
        mvwin ( vi_w_out, 0, mx/2 );
        werase ( vi_w_out );

        wresize ( vi_iw_out, 10+2-1, mx/2 );
        mvwin ( vi_iw_out, 1, mx/2 );
        werase ( vi_iw_out );

        wresize ( log_w, my- ( 10+2 )-3, mx/2);//mert 2 vel osztjuk
        mvwin ( log_w, 10+2, 0 );
        werase ( log_w );

        wresize ( log_iw, my- ( 10+2 )-3-1, mx/2 );
        mvwin ( log_iw, 10+2+1, 0 );
        werase ( log_iw );
	
       	wresize ( slow_log_w, my- ( 10+2 ) - 3, mx/2 );
	mvwin ( slow_log_w, 10+2, mx/2);
	werase ( slow_log_w);

    	wresize ( slow_log_iw, my- ( 10+2 ) - 3 -1, mx/2 );
	mvwin ( slow_log_iw, 10+2+1, mx/2);
	werase ( slow_log_iw);
		
        wresize ( shell_w, 3, mx );
        mvwin ( shell_w, 10+2+my- ( 10+2 ) - 3, 0 );
        werase ( shell_w );
        
	wresize ( shell_iw, 3-1, mx );
        mvwin ( shell_iw, 10+2+my- ( 10+2 ) - 3+1, 0 );
        werase ( shell_iw );

        mvwprintw ( vi_w, 0, 0, "Amminadab's visual imagery " );
	
        mvwprintw ( vi_w_out, 0, 0, "Amminadab's out " );


        mvwprintw ( log_w, 0, 0, "Amminadab's answers" );

        mvwprintw ( slow_log_w, 0, 0, "Amminadab's slow answers" );

        mvwprintw ( shell_w, 0, 0, " Caregiver shell " );
        mvwprintw ( shell_iw, 0, 1, "Norbi> Type your sentence and press [ENTER]" );

        wrefresh ( vi_w );
        wrefresh ( vi_iw );

	wrefresh ( vi_w_out );
        wrefresh ( vi_iw_out );
	
        wrefresh ( log_w );
        wrefresh ( log_iw );

        wrefresh ( slow_log_w );
        wrefresh ( slow_log_iw );

        wrefresh ( shell_w );
        wrefresh ( shell_iw );
      }
  }

  std::mutex ncurses_mutex;
  std::string buf;
  WINDOW *vi_w, *vi_iw;
  WINDOW *log_w, *log_iw;
  WINDOW *shell_w, *shell_iw;

  WINDOW *slow_log_w, *slow_log_iw;
  WINDOW *vi_w_out, *vi_iw_out;

  int mx {0}, my {0};
};

#endif
