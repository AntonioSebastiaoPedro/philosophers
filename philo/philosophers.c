#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void	*print_message(void *message)
{
	char	*str;

	str = (char *)message;
	printf("%s\n", str);
	return (NULL);
}

int	main(void)
{
	char	*message1;
	char	*message2;
	int	i;

	pthread_t thread1, thread2;
	message1 = "Hello from Thread 1!\n";
	message2 = "Hello from Thread 2!\n";
	pthread_create(&thread1, NULL, print_message, (void *)message1);
	pthread_create(&thread2, NULL, print_message, (void *)message2);
	pthread_detach(thread1);
	pthread_detach(thread2);
	i = 5;
	while (i--)
	{
		printf("Thread principal\n");
	}
	return (0);
}
