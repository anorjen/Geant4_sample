void	ft_clock(int c)
{
	int	k;

	k = c % 4;
	if (c == 1)
		write(1, "--", 2);
	else if (k == 1)
		write(1, "\b\b--", 4);
	else if (k == 2)
		write(1, "\b\b\\ ", 4);
	else if (k == 3)
		write(1, "\b\b |", 4);
	else if (k == 0)
		write(1, "\b\b /", 4);
}

void	ft_line(int c)
{
	int		len = 30;
	char	line[len];
	char	r_line[len];
	char	s;

	c = c % (len + 1);
	for(int i = 0; i < len; i++)
	{
		if (c > 0)
			line[i] = '*';
		else
			line[i] = ' ';
		c--;
		r_line[i] = '\b';
	}
	write(1, r_line, len);
	write(1, line, len);
}

void	ft_ball(int c)
{
	int		len = 30;
	char	line[len];
	char	r_line[len];

	c = c % (len * 2);
	if (c > (len - 1))
		c = abs(c - (2 * len - 1));
	for(int i = 0; i < len; i++)
	{
		if (i == c)
			line[i] = 'O';
		else
			line[i] = '.';
		r_line[i] = '\b';
	}
	write(1, r_line, len);
	write(1, line, len);
}
