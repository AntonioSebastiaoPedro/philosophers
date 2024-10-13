#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	ft_putchar(int ch)
{
	write(1, &ch, 1);
}

int	main(void)
{
	char s[1025];
	int i;

	scanf("%[^\n]", s);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] >= 65 && s[i] <= 90)
			ft_putchar(s[i] + 32);
		else if (s[i] >= 97 && s[i] <= 122)
			ft_putchar(s[i] - 32);
		ft_putchar(s[i]);
		i++;
	}
	return (0);
}