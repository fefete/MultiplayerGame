#include "world.h"
#include <thread>
#include "assets.h"
#include "PlayerCharacter.h"
#include <sstream>


namespace iNetConf {
  static char* server = "127.0.0.1";
  int port_tcp = 53000;
  int port_udp = 53035;
  bool connected = false;
}

World* World::m_current_world;

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
            id = com[2];
            logged_in = true;
            new std::thread(&World::recieveUDPPacket, this);
            new std::thread(&World::sendUDPPacket, this);
            //new std::thread(&World::feedUDPSendPackage, this);
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
            p1->name = std::string(com[2]);
            p1->init(pos, size, box_t);
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
  while (true) {
    if (m_send_p_player == nullptr) {
      std::stringstream oss;
      netUDPPackage* aux = new netUDPPackage();
      oss << id << ":" <<
        std::boolalpha << localPlayer->input_right << ":" <<
        std::boolalpha << localPlayer->input_left << ":" <<
        std::boolalpha << localPlayer->input_jump << ":" <<
        std::boolalpha << localPlayer->input_dash << ":" <<
        localPlayer->body_->GetPosition().x << ":" <<
        localPlayer->body_->GetPosition().y << ":" <<
        std::endl;
      aux->system_message = oss.str();
      m_send_p_player = aux;
    }
  }
}

void World::worldInit(b2Vec2 gravity)
{
  m_window_ = new sf::RenderWindow(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "SFML test!");
  m_window_->setVerticalSyncEnabled(true);
  m_window_->setFramerateLimit(60);

  ImGui::SFML::Init(*m_window_);

  m_physics_world_ = new b2World(gravity);

  logged_in = false;

  m_send_p_player = nullptr;
  m_send_p_ball = nullptr;
  m_send_p_system = nullptr;
  m_recieve_TCP = nullptr;
  m_recieve_UDP = nullptr;
  localPlayer = nullptr;

  //connect(&m_tcp, "127.0.0.1", iNetConf::port_tcp);

  //m_tcp.setBlocking(false);
  //m_udp.setBlocking(false);

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
  sf::Font font;
  if (!font.loadFromFile(ASSETS::FontPath("arial.ttf"))) {
    assert(false, "NO .TTF");
  }
  text.setFont(font);

  //ball init
  b = new Ball();
  {
    vec2D pos;
    float radius = 16;
    pos.x = 550;
    pos.y = 200;
    b->init(pos, radius, ball_t);
  }

  /*
    text.setString("Hi");
  text.setCharacterSize(24);
  text.setColor(sf::Color::Red);*/

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
  std::stringstream oss;
  netUDPPackage* aux = new netUDPPackage();
  oss << id << ":" <<
    std::boolalpha << localPlayer->input_right << ":" <<
    std::boolalpha << localPlayer->input_left << ":" <<
    std::boolalpha << localPlayer->input_jump << ":" <<
    std::boolalpha << localPlayer->input_dash << ":" <<
    std::boolalpha << localPlayer->can_jump << ":" <<
    localPlayer->body_->GetPosition().x << ":" <<
    localPlayer->body_->GetPosition().y << ":" <<
    std::endl;
  aux->system_message = oss.str();
  m_send_p_player = aux;
  if (m_recieve_UDP != nullptr) {

    std::vector<std::string> server_status = split(m_recieve_UDP->system_message, ':');
    if (server_status[0] == "update") {
      for (int i = 1; i < server_status.size(); i += 4) {
        if (server_status[i] == "\0") {
          break;
        }
        PlayerCharacter* aux = nullptr;
        for (int b = 0; b < m_characters_in_world.size(); b++) {
          if (m_characters_in_world[b]->name == server_status[i]) {
            aux = m_characters_in_world[b];
            if (aux) {
              aux->movement_state_.clear();
              if (server_status[i + 1] == "True") {
                aux->movement_state_.push_back(kPlayerInputState_right);
              }
              if (server_status[i + 2] == "True") {
                aux->movement_state_.push_back(kPlayerInputState_left);
              }
              if (server_status[i + 3] == "True") {
                aux->movement_state_.push_back(kPlayerInputState_jump);
              }
              if (server_status[i + 4] == "True") {
                aux->movement_state_.push_back(kPlayerInputState_dash);
              }
              if (server_status[i + 5] == "True") {
                aux->can_jump = true;
              }
              else {
                aux->can_jump = false;
              }
            }
            break;
          }
        }
      }
    }
    delete m_recieve_UDP;
    m_recieve_UDP = nullptr;
  }
}



//printf("%s", aux->system_message.c_str());

/*if (m_send_p_ball == nullptr) {
  netPackage* p = new netPackage();
  p->dt_ = last_delta;
  p->id = -1;
  p->pos_x = b->body_->GetPosition().x;
  p->pos_y = b->body_->GetPosition().y;
  m_send_p_ball = p;
}
if (m_send_p_player == nullptr) {
  netPackage* p = new netPackage();
  p->dt_ = last_delta;
  p->id = m_player_char->id;
  p->pos_x = m_player_char->body_->GetPosition().x;
  p->pos_y = m_player_char->body_->GetPosition().y;
  m_send_p_player = p;*/


void World::worldDisconnect()
{
  m_tcp.disconnect();
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
  return text;
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
    ImGui::PopStyleColor(3);
    ImGui::End(); // end window
    //ImGui::ShowTestWindow();
  }
  ImGui::Render();
  m_window_->resetGLStates();
  m_window_->display();
}

Ball* World::getBall() {
  return b;
}