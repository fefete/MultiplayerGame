#include "world.h"
#include <thread>
#include "assets.h"
#include "PlayerCharacter.h"
#include <sstream>


namespace iNetConf {
  char server[255];
  int port_tcp = 53000;
  int port_udp = 53035;
  bool connected = false;
}

World* World::m_current_world;
bool World::disconnect = false;


World::World()
{
}

World::~World()
{
}

std::vector<std::string> split(std::string str, char delimiter) {
  std::vector<std::string> internal;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string tok;

  while (getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

void World::sendTCPPacket() {
  while (true) {
    if (m_send_p_system != nullptr) {
      sendTCP(&m_tcp, *m_send_p_system);
      delete m_send_p_system;
      m_send_p_system = nullptr;
    }
  }
}

void World::recieveTCPPacket() {
  while (true) {
    if (m_recieve_TCP == nullptr) {
      //m_recieve_TCP = new netTCPPackage();
      m_recieve_TCP = recieveTCP(&m_tcp, m_recieve_TCP);
      if (m_recieve_TCP != nullptr) {
        std::vector<std::string> com = split(m_recieve_TCP->system_message, ':');
        if (com[0] == "reg") {
          if (com[1] == "OK") {
          }
          else {
            printf("\nREG FAILURE\n");
          }
        }
        if (com[0] == "login") {
          if (com[1] == "OK") {
            id = atoi(com[2].c_str());
            logged_in = true;
            new std::thread(&World::recieveUDPPacket, this);
            new std::thread(&World::sendUDPPacket, this);
          }
          else {
            printf("\nLOG FAILURE\n");
          }
        }
        if (com[0] == "game") {
          if (com[1] == "newPlayer") {
            PlayerCharacter* p1 = new PlayerCharacter();
            vec2D pos;
            pos.x = atof(com[3].c_str()) * SCALE;
            pos.y = atof(com[4].c_str()) * SCALE;
            vec2D size;
            size.x = 32;
            size.y = 32;
            p1->id = atoi(com[2].c_str());
            p1->init(pos, size, box_t_2);
          }
          else {
          }
        }
        delete m_recieve_TCP;
        m_recieve_TCP = nullptr;
      }
    }
  }

}


void World::sendUDPPacket() {
  while (true)
  {
    if (m_send_p_player != nullptr) {
      sendUDP(&m_udp, iNetConf::server, iNetConf::port_udp, *m_send_p_player);
      delete m_send_p_player;
      m_send_p_player = nullptr;
    }
    if (m_send_p_ball != nullptr) {
      sendUDP(&m_udp, iNetConf::server, iNetConf::port_udp, *m_send_p_ball);
      delete m_send_p_ball;
      m_send_p_ball = nullptr;
    }
  }
}
void World::recieveUDPPacket() {
  while (true)
  {
    if (m_recieve_UDP == nullptr) {
      //printf("REC UDP \n");
      sf::IpAddress sender;
      m_recieve_UDP = recieveUDP(&m_udp, sender, iNetConf::port_udp);
    }
  }
}

void World::feedUDPSendPackage() {

}

void World::worldInit(b2Vec2 gravity)
{
  m_window_ = new sf::RenderWindow(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "SFML test!");
  m_window_->setVerticalSyncEnabled(true);
  m_window_->setFramerateLimit(60);

  ImGui::SFML::Init(*m_window_);

  m_physics_world_ = new b2World(gravity);

  m_physics_world_->SetAllowSleeping(false);

  logged_in = false;

  m_send_p_player = nullptr;
  m_send_p_ball = nullptr;
  m_send_p_system = nullptr;
  m_recieve_TCP = nullptr;
  m_recieve_UDP = nullptr;
  localPlayer = nullptr;

  //connect(&m_tcp, "127.0.0.1", iNetConf::port_tcp);

  //m_tcp.setBlocking(false);
  m_udp.setBlocking(false);

  new std::thread(&World::recieveTCPPacket, this);
  new std::thread(&World::sendTCPPacket, this);
  m_cb_ = new CollisionCallback();
  m_physics_world_->SetContactListener(m_cb_);


  memset(username, ' ', 255);
  memset(password, ' ', 255);

  if (!ground_t.loadFromFile(ASSETS::ImagePath("ground.png"))) {
    assert(false, "NOT IMAGE FOR GROUND");
  }
  if (!box_t.loadFromFile(ASSETS::ImagePath("box.png"))) {
    assert(false, "NOT IMAGE FOR PLAYER");
  }
  if (!box_t_2.loadFromFile(ASSETS::ImagePath("box_2.png"))) {
    assert(false, "NOT IMAGE FOR PLAYER");
  }
  if (!wall_t.loadFromFile(ASSETS::ImagePath("wall.png"))) {
    assert(false, "NOT IMAGE FOR WALL");

  }
  if (!side_wall_t.loadFromFile(ASSETS::ImagePath("b_wall.png"))) {
    assert(false, "NOT IMAGE FOR SIDE WALL");
  }

  if (!ball_t.loadFromFile(ASSETS::ImagePath("ball.png"))) {
    assert(false, "NOT IMAGE FOR BALL");
  }

  //load fonts
  if (!font.loadFromFile(ASSETS::FontPath("arial.ttf"))) {
    assert(false, "NO .TTF");
  }
  text_score_1.setFont(font);
  text_score_2.setFont(font);

  text_score_1.setStyle(sf::Text::Bold | sf::Text::Underlined);
  text_score_2.setStyle(sf::Text::Bold | sf::Text::Underlined);

  text_score_1.setColor(sf::Color::Blue);
  text_score_2.setColor(sf::Color::Red);

  text_score_1.setCharacterSize(24);
  text_score_2.setCharacterSize(24);

  //ball init
  m_b = new Ball();
  {
    vec2D pos;
    float radius = 16;
    pos.x = 550;
    pos.y = 200;
    m_b->init(pos, radius, ball_t);
  }


}

void World::wordReadInput()
{
  for (unsigned int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->readInput();
  }
}

void World::worldPollEvents()
{
  sf::Event event;
  while (m_window_->pollEvent(event))
  {
    ImGui::SFML::ProcessEvent(event);
    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_window_->close();
      worldDisconnect();
    }
  }
}

void World::worldSync()
{
  netUDPPackage* aux = new netUDPPackage();
  aux->name = id;
  aux->x = localPlayer->body_->GetPosition().x;
  aux->y = localPlayer->body_->GetPosition().y;
  aux->v_x = localPlayer->body_->GetLinearVelocity().x;
  aux->v_y = localPlayer->body_->GetLinearVelocity().y;
  aux->has_ball = localPlayer->has_ball;
  aux->dash_cd = localPlayer->dash_cd;
  m_send_p_player = aux;
  if (m_recieve_UDP != nullptr) {

    for (int i = 0; i < m_characters_in_world.size(); i++) {
      if (m_characters_in_world[i]->id == m_recieve_UDP->name) {
        b2Vec2 pos = b2Vec2(m_recieve_UDP->x, m_recieve_UDP->y);
        m_characters_in_world[i]->body_->SetTransform(pos, 0);
        m_characters_in_world[i]->dash_cd = m_recieve_UDP->dash_cd;

      }
    }
    delete m_recieve_UDP;
    m_recieve_UDP = nullptr;
  }
}


void World::worldDisconnect()
{
  disconnect = true;
}

int World::numberOfCharactersInPlay()
{
  return m_characters_in_world.size();
}

sf::Texture* World::playerTexture()
{
  return &box_t;
}

sf::Texture* World::wallTexture()
{
  return &wall_t;
}

sf::Texture* World::groundTexture()
{
  return &ground_t;
}

sf::Texture* World::ballTexture()
{
  return &ball_t;
}

sf::Texture* World::sideWallTexture()
{
  return &side_wall_t;
}

sf::Text World::fontTexture()
{
  return sf::Text();
}

void World::setBall(Ball *b)
{
  m_b = b;
}

void World::worldSetSendSystemData(std::string s)
{
  if (m_send_p_system == nullptr) {
    netTCPPackage* p = new netTCPPackage();
    p->system_message = s;
    m_send_p_system = p;
  }
}

void World::addCharacterToWorld(Entity * entity)
{
  m_entites_to_draw.push_back(entity);
  if (entity->type_ == kEntityType_player) {
    PlayerCharacter* p = dynamic_cast<PlayerCharacter*>(entity);
    if (p) {
      m_characters_in_world.push_back(p);
    }
  }
}

void World::deleteCharacterFromWorld()
{
}

sf::RenderWindow* World::getWindow()
{
  return m_window_;
}

b2World * World::getPhysicsWorld()
{
  return m_physics_world_;
}

World * World::getWorld()
{
  if (!m_current_world) {
    m_current_world = new World();
  }
  return m_current_world;
}

void World::worldUpdate(float dt)
{
  for (unsigned int i = 0; i < m_characters_in_world.size(); i++) {
    m_characters_in_world[i]->update(dt);
  }
  std::string s = std::to_string(m_characters_in_world[0]->score);
  text_score_1.setString(s);
  if(m_characters_in_world.size() > 1){
    std::string s2 = std::to_string(m_characters_in_world[1]->score);
    text_score_2.setString(s2);
  }
  if (m_b != nullptr) {
    m_b->update(dt);
  }
  if (disconnect) {
    m_tcp.disconnect();
    m_udp.unbind();
    m_window_->close();
  }
}


void World::worldDraw()
{
  m_window_->clear(sf::Color::White);
  if (logged_in) {
    for (unsigned int i = 0; i < m_entites_to_draw.size(); i++) {
      if (m_entites_to_draw[i]->m_can_draw == false) {
        continue;
      }
      m_entites_to_draw[i]->draw();
      m_window_->draw(m_entites_to_draw[i]->sprite_);
    }
    m_window_->draw(text_score_1);
    if (m_characters_in_world.size() > 1){
        m_window_->draw(text_score_2);
        text_score_2.setPosition(600, 0);
    }
  }
  else {

    ImGui::Begin("LOG IN WINDOW");
    // Window title text edit
    ImGui::InputText("Username", username, 255);
    ImGui::InputText("Password", password, 255);
    ImGui::InputText("server", iNetConf::server, 255);
    ImGui::InputInt("TCP Port", &iNetConf::port_tcp);
    ImGui::InputInt("UDP Port", &iNetConf::port_udp);
    if (ImGui::Button("Register")) {
      std::string user = std::string(username);
      std::string pass = std::string(password);
      std::string final_string = "reg:" + user + ":" + pass + ":";
      World::getWorld()->worldSetSendSystemData(final_string);
      memset(username, ' ', 255);
      memset(password, ' ', 255);
    }
    if (ImGui::Button("Login")) {
      std::string user = std::string(username);
      std::string pass = std::string(password);
      std::string final_string = "login:" + user + ":" + pass + ":";
      World::getWorld()->worldSetSendSystemData(final_string);
      memset(username, ' ', 255);
      memset(password, ' ', 255);
    }
    char* c;

    if (!iNetConf::connected) {
      c = "connect";
      ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(7.0f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(7.0f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(7.0f, 0.8f, 0.8f));
    }
    else {
      ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(0.4f, 0.6f, 0.6f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(0.4f, 0.7f, 0.7f));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(0.4f, 0.8f, 0.8f));
      c = "connected";
    }


    if (ImGui::Button(c)) {
      if (!iNetConf::connected) {
        iNetConf::connected = connect(&m_tcp, iNetConf::server, iNetConf::port_tcp);
      }
      else {
      }
    }

    if (ImGui::Button("GO OFFLINE")) {
      logged_in = true;
    }
    ImGui::PopStyleColor(3);
    ImGui::End(); // end window
    //ImGui::ShowTestWindow();
  }
  ImGui::Render();
  m_window_->resetGLStates();
  m_window_->display();
}

Ball* World::getBall() {
  return m_b;
}