/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 13:36:18 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/15 13:36:21 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	s_len;
	char			*sub;

	i = 0;
	s_len = ft_strlen(s);
	if (s == NULL)
		return (NULL);
	if (start > s_len)
		return (ft_strdup(""));
	if (s_len <= len)
		len = (s_len - start);
	sub = malloc(sizeof(char) * (len + 1));
	if (sub == NULL)
		return (NULL);
	while (i < len && s[i] != '\0' )
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

/*we want to move forward in the string until we reach the first 
non-splitting character */

static size_t	ft_wordcount(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			while (s[i] != c && s[i] != '\0')
				i++;
			count++;
		}
	}
	return (count);
}

/* for each string delimited by c an array gets filled 
 char *ft_substr(char const *s, unsigned int start, size_t len) */

static void	filling_array(char **arr, size_t len_arr, char const *s, char c)
{
	size_t	i;
	size_t	len_word;

	i = 0;
	while (i < len_arr)
	{
		len_word = 0;
		while (*s == c && *s != 0)
			s++;
		while (s[len_word] != c && s[len_word] != '\0')
			len_word++;
		arr[i] = ft_substr(s, 0, len_word);
		s += len_word;
		i++;
	}
	arr[i] = 0;
}

char	**ft_split(char const *s, char c)
{
	size_t	wcount;
	char	**arr;

	if (!s)
		return (NULL);
	wcount = ft_wordcount(s, c);
	arr = malloc(sizeof(char *) * (wcount + 1));
	if (!arr)
		return (NULL);
	filling_array(arr, wcount, s, c);
	return (arr);
}
