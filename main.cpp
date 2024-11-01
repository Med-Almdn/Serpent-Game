#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace std;
typedef struct point{
	int x,y;
}Point;
class Position {
		Point pos;
	public:
		
        Position add(Position a,Position b){    
            Position p;
            p.pos.x=a.pos.x + b.pos.x;
            p.pos.y=a.pos.y + b.pos.y;
            return p;
        }
		
        Position sub(Position a,Position b){     
            Position p;
            p.pos.x=a.pos.x - b.pos.y;
            p.pos.y=a.pos.y - b.pos.y;
            return p;
        }
        point getPos(){
            return pos;
        }
        void setPos(int a, int b){
            pos.x=a; pos.y=b;
        }
};
class Direction {
	public:
		Point up ;
		Point down;
		Point left;
		Point right;
		Point dir ;   
	public:
	
		Point get_dir(int i){
           if(i==1){  dir=left ;return left;  }
           if(i==2){  dir=right;return right; }
           if(i==3){  dir=up  ; return up;    }
           if(i==4){  dir=down; return down;  }
        }
		void setup  () {  up.x=0 ;  up.y=-16; }
        void setdown() {  up.x=0 ;  up.y=16 ; }
        void setleft() {  up.x=-16; up.y=0 ;  }
        void setright(){  up.x=16 ; up.y=0 ;  }
        
		Point getZero(){  Point z ; z.x =0 ; z.y=0; return z;}
};
class Fruit {
	public:
        Point position;
		sf::RectangleShape lefruit;
		int ligne;
		int colon;
	    Fruit(){
            lefruit.setSize(sf::Vector2f(16, 16));
            lefruit.setOutlineColor(sf::Color::Black);
            lefruit.setFillColor(sf::Color(255,42,0));
            lefruit.setOutlineThickness(1.0f);
            lefruit.setPosition(0, 0);
        }
        void draw(sf::RenderWindow &window){
            window.draw(lefruit);
        }
        void change_pos(int a,int b){
            ligne=a; colon=b;
            lefruit.setPosition(a,b);
        	position.x=a; position.y=b;
    	}
        Point getPos(){
	    	return position;
        }
};
class serpent{
        sf::Clock sClock  ;
        int UpdateTime ;
        float cTime;
	    std::vector<sf::RectangleShape> tserp;
	    std::vector<sf::RectangleShape> obstacles;
	    std::vector<sf::RectangleShape> border ;
	    Position tab_border[28];
	    int k;
	    int score;
	    bool mLeft, mRight, mUp, mDown;
		Position position;
		Direction vitesse;
        int longeur; 
		Position Body[200]; 		
		Fruit fruit;
		
		void changer_fruit_pos(sf::RenderWindow &window){
            int randomX ;
            int randomY ;
            int j;
            do{	j=0;
                randomX = rand() % (window.getSize().x/16);
                for(int i=0;i<longeur;i++)
                {	for(int h=0;h<k;h++)    
                        if(randomX*16==Body[i].getPos().x || randomX*16==tab_border[h].getPos().x )
                            j++;
                }
            }while(j>0);
			do{	j=0;
                randomY = rand() % (window.getSize().y/16);
                for(int i=0;i<longeur;i++)
                {	for(int h=0;h<k;h++)    
                        if(randomY*16==Body[i].getPos().y || randomY*16==tab_border[h].getPos().y ) 
                            j++;
                }
            }while(j>0);
            fruit.change_pos(randomX*16,randomY*16);
        } 
        void move_body(){
            if (cTime < UpdateTime)
                cTime += sClock.restart().asMilliseconds();
            else
            {	cTime = 0.0f;
                if (longeur> 1){
                    for (int i = longeur - 1; i > 0; i--){
                       tserp[i].setPosition(sf::Vector2f(tserp[i-1].getPosition().x, tserp[i-1].getPosition().y));
                    }
                }
                if (mLeft)  tserp[0].move(-16, 0);
                if (mRight) tserp[0].move( 16, 0);
                if (mUp)    tserp[0].move( 0,-16);
                if (mDown)  tserp[0].move( 0, 16);
            }
        } 
		void is_injured_itself(sf::RenderWindow &window){
            for (int i = 2; i < longeur; i++){
                if (Body[0].getPos().x == Body[i].getPos().x && Body[0].getPos().y == Body[i].getPos().y){
                    window.close();
                }
            }
        } 
        void is_colliding_wall(sf::RenderWindow &window){
            for(int i=0;i<28;i++){
                if (Body[0].getPos().x == tab_border[i].getPos().x && Body[0].getPos().y == tab_border[i].getPos().y)
					window.close();
                if (Body[0].getPos().x >= window.getSize().x )  tserp[0].setPosition(0,Body[0].getPos().y);
                if (Body[0].getPos().x <  0 )                   tserp[0].setPosition(592,Body[0].getPos().y);
                if (Body[0].getPos().y >= window.getSize().y )  tserp[0].setPosition(Body[0].getPos().x,0);
                if (Body[0].getPos().y <  0 )                   tserp[0].setPosition(Body[0].getPos().x,592);
            }
        }                                                 
	public:
        serpent(sf::RenderWindow &window){
        	score=0;
        	longeur=0; 
			k=0;
            changer_fruit_pos(window);
            UpdateTime = 150;
            cTime = 0.0f;
            srand(std::time(0));
            mLeft = mUp = mDown = false;
            mRight = true;
            vitesse.setdown();
            vitesse.setleft();
            vitesse.setright();
            vitesse.setup();
            for(int i=0;i<14;i++) obst1();
            for(int i=0;i<14;i++) obst2();
        	AddCase();
        }
        int getScore(){
        	return score;
    	}
        void AddCase(){
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(16, 16));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1.0f);
            if (tserp.empty()){
                rect.setFillColor(sf::Color(0,0,179));
                rect.setPosition(320, 256);
                longeur++;
                position.setPos(320,256);
                Body[0]=position;
            }
            else{
                rect.setFillColor(sf::Color(51,51,255));
                rect.setPosition(tserp[longeur - 1].getPosition().x , tserp[longeur - 1].getPosition().y);
                position.setPos(rect.getPosition().x,rect.getPosition().y);
                Body[longeur]=position;
                longeur++;
            }
            tserp.push_back(rect);
        }
        string intToString(int value) {
		    std::ostringstream oss;
		    oss << value;
		    return oss.str();
		}
        void draw(sf::RenderWindow &window){
            fruit.draw(window);
            for (unsigned int i = 0; i < longeur; i++) window.draw(tserp[i]);
            for (int i=0; i<obstacles.size();i++)window.draw(obstacles[i]);
            for (int i=0; i<border.size();i++)window.draw(border[i]);
            sf::Font font;
			font.loadFromFile("Arial.ttf");
			sf::Text scoreText;
			scoreText.setFont(font);
    		scoreText.setCharacterSize(24);
    		scoreText.setFillColor(sf::Color::Black);
    		scoreText.setString("Score: " + intToString(getScore()));
    		scoreText.setPosition(10, 10);
    		window.draw(scoreText);
		    
        }
        void cli(sf::Event &event){
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Left){
                	if(mRight == false){mLeft = true;
                    					mRight = false;
                    					mUp = false;
										mDown = false;
                	}
                }
                if (event.key.code == sf::Keyboard::Right){
                	if(mLeft == false){	mLeft = false;
                    					mRight = true;
										mUp = false;
                    					mDown = false;
                	}
                }
                if (event.key.code == sf::Keyboard::Up){
                	if(mDown == false){	mLeft = false;
	                    				mRight = false;
										mUp = true;
	                    				mDown = false;
	                }
                }
                if (event.key.code == sf::Keyboard::Down){
                	if(mUp == false){	mLeft = false;
	                    				mRight = false;
										mUp = false;
	                    				mDown = true;
	            	}
                }
            }
        }	
        void obst1(){
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(16, 16));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1.0f);
            if (obstacles.empty()){
                rect.setFillColor(sf::Color::White);
                rect.setPosition(400, 192);
                tab_border[k].setPos(rect.getPosition().x,rect.getPosition().y);
                k++;
            }
            else{
                rect.setFillColor(sf::Color::White);
                rect.setPosition(obstacles[obstacles.size() - 1].getPosition().x - rect.getSize().x, obstacles[obstacles.size() - 1].getPosition().y);
                tab_border[k].setPos(rect.getPosition().x,rect.getPosition().y);
                k++;
            }
            obstacles.push_back(rect);
        }
        void obst2(){
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(16, 16));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1.0f);
            if (border.empty()){
                rect.setFillColor(sf::Color::White);
                rect.setPosition(400, 416);
                tab_border[k].setPos(rect.getPosition().x,rect.getPosition().y);
                k++;
            }
            else{
                rect.setFillColor(sf::Color::White);
                rect.setPosition(border[border.size() - 1].getPosition().x - rect.getSize().x, border[border.size() - 1].getPosition().y);
                tab_border[k].setPos(rect.getPosition().x,rect.getPosition().y);
                k++;
            }
            border.push_back(rect);
        }       
        void move(sf::RenderWindow &window){
            move_body();
            for(int i=0;i<longeur;i++) Body[i].setPos(tserp[i].getPosition().x,tserp[i].getPosition().y);
            is_injured_itself(window);
            is_colliding_wall(window);
            if (Body[0].getPos().x == fruit.getPos().x && Body[0].getPos().y == fruit.getPos().y)
			{
                AddCase();
                changer_fruit_pos(window);
                score++;
            }
        }      
};

int main(){
    sf::RenderWindow window(sf::VideoMode(608, 608), "Serpent Game");
    window.setFramerateLimit(64);
    window.setKeyRepeatEnabled(false);
	serpent s(window);
	while (window.isOpen()){
        s.move(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        	s.cli(event);
        }
        window.clear(sf::Color(0,153,0));
        s.draw(window);
        window.display();
    }
    return 0;
}
