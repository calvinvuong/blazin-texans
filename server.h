void print_IP(unsigned int queue[], int size);

int listener(int sd, unsigned int *ip_queue, int *queue_size, int timeout);

int transfer_IPs(unsigned int *ip_queue, int *queue_size, unsigned int *player_IPs, int num_players);

int update_game_status(int pid);

int game(unsigned int player_IPs[], int num_players);


