void insert(loglist **head, logline *log)
{
	loglist_Ptr listItr = (*head), // Used to iterate through the nodes of the list
						temp;	  // Used to allocate memory for linked list and to act as temporary node pointer

	if ((*head) == NULL) // Initial case for creating the first node, or the head of the list
	{
		temp = (loglist_Ptr)malloc(sizeof(loglist_t)); // Allocate memory
    
    // Copy contents of the struct argument into the struct line of loglist struct
		strcpy(line->level, log->level);
    strcpy(line->timestamp, log->timestamp);
    strcpy(line->message, log->message);
    
		temp->next = NULL;			// Point to the end of the list
		(*head) = temp;				// Initalize the head to NULL
		return;
	}

	// Not the end of the list AND the symbols don't match, then iterate through the list
	while (listItr->next != NULL)
	{
		listItr = listItr->next;
	}
 
	 // The while loop is terminated because iterator reached the end of the list
		temp = (loglist_Ptr)malloc(sizeof(loglist_t));		// Allocate memory
		
   `// Copy contents of the struct argument into the struct line of loglist struct
		strcpy(line->level, log->level);
    strcpy(line->timestamp, log->timestamp);
    strcpy(line->message, log->message);
    
		temp->next = NULL;				// Point to the end of the list
		listItr->next = temp;			// Initalize the next node to NULL
}