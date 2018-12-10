#include "imap.hpp"


/*Function in Message class*/
string Message::getBody(){

}

string Message::getField(){

}

void Message::deleteFromMailbox(){}

/*Function in Session class*/
Message** Session::getMessages(){
 
  set=mailiamp_set_new_interval(1,0);
  fetch_type=mailiamp_fetch_type_new_fetch_att_list_empty();
  
  fetch_att=mailiamp_fetch_att_new_uid();
  
  r=mailiamp_fetch_type_new_fetch_att_list_add(fetch_type,fetch_att);
  check_error(r,"could not add to list");

  r=mailiamp_fetch(imap,set,fetch_type, &fetch_result);
  check_error(r,"could not get messages");

  for(cur=clist_begin(fetch_result);cur!=NULL;cur=clist_next(cur)){
    struct mailiamp_msg_att* msg_att;
    unsigned int uid;//stoped here

    msg_att=clist_content(cur);
    uid=
  }
  
}

void Session::connect(std::string const& server, size_t port = 143){
  imap=mailimap_new(0,NULL);//remember to free after logout,returns a new IMAP session
  r=mailimap_socket_connect(imap, server.c_str(), port);
  fprintf(stderr, "connect: %i\n",r);
  check_error(r,"could not connect");
  
}

void Session::login(std::string const& userid, std::string const& password){
  r=mailimap_login(imap, userid.c_str(), password.c_str());
  check_error(r, "could not login");  
}

void Session::selectMailbox(std::string const& mailbox){
  mailiamp_select(imap, mailbox.c_str());
  check_error(r,"could not select %s\n", mailbox.c_str());
}


Session::Session(){
  mailiamp_fetch_list_free(fetch_result);
  mailiamp_set_free(set);
  mailimap_logout(imap);
  mailiamp_free(imap);
}
