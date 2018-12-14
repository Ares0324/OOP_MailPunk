#include "imap.hpp"


/*Function in Message class*/
std::string IMAP::Message::getBody(){
  struct mailiamp_set* set;
  struct mailiamp_fetch_type* fetch_type;
  struct mailiamp_fetch_att* fetch_att;
  clist* fetch_result;
  clistiter* cur;
  
  set=mailiamp_set_new_single(uid);
  fetch_type=mailiamp_fetch_type_new_fetch_att_list_empty();
  fetch_att=mailiamp_fetch_att_new_body();
  mailiamp_fetch_type_new_fetch_att_list_add(fetch_type, fetch_att);
  r=mailiamp_fetch(imap, set, fetch_type, &fetch_result);
  check_error(r, "could not fetch body");

  //get body;
  struct mailiamp_msg_att* msg_att;
  msg_att=clist_content(fetch_result);
  body=NULL;

  for(cur=clist_begin(msg_att->att_list); cur!=NULL; cur=clist_next(cur)){
    struct mailiamp_msg_att_item* item;
    item=clist_content(cur);
    if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC) continue;		
    if (item->att_data.att_static->att_type != MAILIMAP_MSG_ATT_BODY_SECTION) continue;

    body=item->att_data.att_static->att_data.att_body_section->sec_body_part; break;    
  }

  mailiamp_fetch_list_free(fetch_result);
  mailiamp_set_free(set);
  return body;
}


std::string IMAP::Message::getField(std:: string fieldname){
  std::string field=NULL;
  struct mailiamp_set* set;
  struct mailiamp_header_list* headers;
  struct mailiamp_section* section;
  clist* fetch_result;
  clistiter* cur;

  set=mailiamp_set_new_single(uid);
  section=mailiamp_section_new_header_fields(headers);

  

  mailiamp_set_free(set);
  return field;
}

void IMPA::Message::deleteFromMailbox(){
  struct mailiamp_set* set;
  struct mailiamp_flag* d;
  struct mailiamp_flag_list* flag_list;
  struct mailiamp_store_att_flags* store;
  int r;

  set=mailiamp_set_new_single(uid);
  flag_list=mailiamp_flag_list_new_empty();
  d=mailiamp_flag_new_deleted();
  
  r=mailiamp_flag_list_add(flag_list,d);
  check_error(r,"could not mark as \Deleted");

  store=mailiamp_store_att_flags_new_set_flags(flag_list);
  
  mailiamp_uid_store(session->imap,set,store);
 
  r=mailiamp_expunge(session->imap);
  check_error(r,"could not delete message");
  mailiamp_store_att_flags_free(store);

  mailiamp_set_free(set);
}



/*Function in Session class*/
Message** IMAP::Session::getMessages(){
  struct mailiamp_set* set;
  struct mailiamp_fetch_type* fetch_stype;
  struct mailiamp_fetch_att* fetch_att;
  clist* fecth_result;
  clist* cur;
  
  int r;//for check_error
  set=mailiamp_set_new_interval(1,0);
  fetch_type=mailiamp_fetch_type_new_fetch_att_list_empty();//creates a list of fetch_stype
  
  fetch_att=mailiamp_fetch_att_new_uid();//creates a fetch_att to request uid
  
  mailiamp_fetch_type_new_fetch_att_list_add(fetch_type,fetch_att);//add fetch_att to fetch_tyoe

  r=mailiamp_fetch(imap,set,fetch_type, &fetch_result);//clist: fetch_result
  check_error(r,"could not get messages");

  for(cur=clist_begin(fetch_result);cur!=NULL;cur=clist_next(cur)){
    struct mailiamp_msg_att* msg_att;
    unsigned int uid=0;// get uid for each message
    msg_att=clist_content(cur);

    for(clistiter* cur=clist_begin(msg_att->att_list); cur!=NULL; cur=clist_next(cur)){
        struct mailimap_msg_att_item * item;		
       	item = clist_content(cur);
	if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC) continue;
		
       	if (item->att_data.att_static->att_type != MAILIMAP_MSG_ATT_UID) continue;
        uid=item->att_data.att_static->att_data.att_uid; break;
    }
    
    if(uid==0) continue;
  }
  
   return fetch_result;
}


  void IMAP::Session::connect(std::string const& server, size_t port = 143){
   int r;//for check_error
  imap=mailimap_new(0,NULL);//remember to free after logout,returns a new IMAP session
  r=mailimap_socket_connect(imap, server.c_str(), port);
  fprintf(stderr, "connect: %i\n",r);
  check_error(r,"could not connect");
  
}

  void IMAP::Session::login(std::string const& userid, std::string const& password){
  int r;//for check_error
  r=mailimap_login(imap, userid.c_str(), password.c_str());
  check_error(r, "could not login");  
}

  void IMAP::Session::selectMailbox(std::string const& mailbox){
  int r;//for check_error
  
  mailiamp_select(imap, mailbox.c_str());
  check_error(r,"could not select %s\n", mailbox.c_str());
}


  IMAP::Session::Session(){
  mailiamp_fetch_list_free(fetch_result);
  mailiamp_set_free(set);
  mailimap_logout(imap);
  mailiamp_free(imap);
}
