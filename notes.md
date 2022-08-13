
loop {
	if (message in message_queue())
		subscrip_to_pollout()
	
	poll()

	loop on pollfd vector {
		if revent is POLLIN
			get_msg
		if revent is POLLOUT
			send_msg
			msg_sent
	}

	if (message_queue.front.sent())
		message_queue.pop()

}