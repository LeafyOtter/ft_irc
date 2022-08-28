#include "Server.hpp"
#include <iostream>
#include "utils_chan.cpp"
#include "../errors.hpp"
#include "../replies.hpp"
/*

Command: NAMES
   Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

        A VERIFIER :
                - By using the NAMES command, a user can list all nicknames that are
                For more details on what is visible and what is not,
                see "Internet Relay Chat: Channel Management" [IRC-CHAN].  
                -  Wildcards are allowed in the <target> parameter -> pas compris
                - quels sont les channels visibles

        A FAIRE :
                - If no <channel> parameter is given, a list of all channels and their
                occupants is returned.  At the end of this list, a list of users who
                are visible but either not on any channel or not on a visible channel
                are listed as being on `channel' "*".

                - If the <target> parameter is specified, the request is forwarded to
                that server which will generate the reply.

                - afficher les channel demandé par l'user

  
    Numeric error and Replies:   * -> dans le fichier erreur // ** -> utilisé

        There is no error reply for bad channel names.
        ERR_TOOMANYMATCHES (a verif)              *ERR_NOSUCHSERVER
        RPL_NAMREPLY (a faire plus tard)          RPL_ENDOFNAMES (a faire plus tard)

*/
namespace c_irc
{
	void Server::cmd_names(int fd, arguments_t &args)
	{
		c_irc::User &user = *users[fd];
                //std::string name = "NAMES";
                if (not user.is_mode(U_MODE_REGISTERED_PASS))
			return ;
                std::cout << "cmd names" << std::endl; 
		std::cout << fd << &args << std::endl; 
                if (args.size() < 1)
                {        
                        // si aucun param -> list tous les channel visible (?) + leurs user
                }
                // si plus d'arg -> split la ligne et verif si les channel existent et afficher leurs user
	}
} 