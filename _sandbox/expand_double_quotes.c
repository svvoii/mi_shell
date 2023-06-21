/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_double_quotes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 20:11:17 by sbocanci          #+#    #+#             */
/*   Updated: 2023/06/21 15:27:23 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

/*
The code in c. Each function has to be under 25 lines of code.
It suppose to expand the string if it is double quoted or not quoted and replace the envp key with its value.
The key is denoted dy $ sign. For example: [$KEY] shall be replaced with [VALUE] form envp environment variables.

The environment variavbles are stored in (cahr **m_envp) and can be accessed by indexing into: (m_envp[index]).

The input data is stored in (char **arguments).
In the examnple of this input line:
input: ```echo $USER Hello ". $USER  .."  $NOT_IN_ENVP '  follow the $USER..  ' THEN..$'USER' .. $"USER"```

the `char **arguments` will look like this:
```
arguments[0]: [echo]
arguments[1]: [$USER ]
arguments[2]: [Hello ]
arguments[3]: [". $USER  .."  ]
arguments[4]: [$NOT_IN_ENVP ]
arguments[5]: ['  follow the $USER..  ' ]
arguments[6]: [THEN..$]
arguments[7]: ['USER' ]
arguments[8]: [.. ]
arguments[9]: [$]
arguments[10]: ["USER"]
arguments[11]: NULL;
```
The goal is to check if there are any $ signs within all strings in `**arguments`
and replace the `$KEY` to `VALUE` from `**m_envp` if it is not singlequoted.
If the string appears in single quotes it shall be treated as it is.
Original spaces shall be precerved in the end of string if available.

The new `**arguments` shall appear like this:
```
arguments[0]: [echo]
arguments[1]: [user_name ]
arguments[1]: [Hello ]
arguments[2]: [. user_name  ..  ]
arguments[3]: [ ]
arguments[4]: [  follow the $USER..   ]
arguments[5]: [THEN..]
arguments[5]: [USER ]
arguments[5]: [.. ]
arguments[5]: [USER]
arguments[6]: NULL;
```
So, eventualy the output shall be like
bash output: ```user_name Hello . user_name  ..   follow the $USER..   THEN..USER .. USER```
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for getting the value of env variable (given a certain key)
char *get_env_var(char *key, char **m_envp)
{
	int len = strlen(key);
	// Loop through the env variables
	while (*m_envp)
	{
		// Look for env variables that match the key
		if (strncmp(*m_envp, key, len) == 0 && *(*m_envp + len) == '=')
		{
			return ++(*m_envp);
		}
		m_envp++; // Move to next env variable
	}

	return NULL; // If not found
}

char *replace_env_var(char *input, char **m_envp)
{
	int len;
	int i;
	int j;
	int start;
	char *output;
	char key[1024];
	char value[1024];

	i = 0;
	j = 0;
	start = 0;
	output = (char *)malloc(len * sizeof(char));
	memset(output, 0, len);
	len = strlen(input);
	while (i < len)
	{
		if (input[i] == '$' && (i == 0 || input[i - 1] != '\\')) // If dollar sign is found and previous char is not backslash
		{
			start = i + 1; // Mark the beginning of variable name
			i++;		   // Move to next char

			// If next char is not a special char
			if (!(input[i] == '\\' || input[i] == '"' || input[i] == '\''))
			{
				// Find the end of variable name
				while (i < len && input[i] != ' ' && input[i] != '"' && input[i] != '\'')
					i++;
				// Extract the variable name
				// key[i - start + 1];
				memcpy(key, &input[start], i - start);
				key[i - start] = '\0';

				// Get variable value from environment variables
				// char* var_value = get_env_var(key, m_envp);
				*value = get_env_var(key, m_envp);

				// If variable value found, append it to output
				if (value)
				{
					strcat(output, value);
					j += strlen(value);
				}
			}
		}
		else
		{
			output[j] = input[i];
			i++;
			j++;
		}
	}
	// Free the memory pointed by input to be able to assing output instead
	// free(input);
	return output;
}

char **prepare_command(char **arguments, char **m_envp)
{
	int i = 0;
	while (arguments[i])
	{
		arguments[i] = replace_env_var(arguments[i], m_envp);
		i++;
	}
	return arguments;
}

// echo Hello ". $USER  .."  $NOT_IN_ENVP '  follow the $USER..  ' THEN..$'USER' .. $"USER"
int main(int argc, char **argv, char **envp)
{
	char **arguments = (char *[]){"echo", "Hello", "\". $USER  ..\"", "$FIRST", "'  follow the white $USER..  '", "THEN..", NULL};
	char **m_envp = envp;

	int i = 0;
	while (arguments[i])
		printf("[%d]: [%s]\n", i, arguments[i++]);
	printf("\n");

	arguments = prepare_command(arguments, m_envp);

	i = 0;
	while (arguments[i])
		printf("[%d]: [%s]\n", i, arguments[i++]);
	printf("\n");

	return 0;
}

/*
char* get_env_var(char* key, char** m_envp)
{
	while(*m_envp) {
		if (strncmp(*m_envp, key, strlen(key)) == 0)
			return strchr(*m_envp, '=') + 1;
		m_envp++;
	}
	return NULL;
}

char* replace_env_var(char* str, char** envp)
{
	char* outStr = malloc(1024 * sizeof(char));
	int i = 0, j = 0, keyStart, endOfString = 0;
	char* key;

	while(str[i]){
		if (str[i] == '\'') {
			outStr[j] = str[i];
			i++;
			while(str[i] && str[i] != '\''){
				outStr[j] = str[i];
				i++;
				j++;
			}
			if (str[i]) {
				outStr[j] = str[i];
				i++;
				j++;
			}
			continue;
		}
		if (str[i] == '\"') {
			outStr[j] = str[i];
			i++;
			while(str[i] && str[i] != '\"'){
				if (str[i] == '$') {
					i++;
					keyStart = i;
					while (str[i] && str[i] != ' ' && str[i] != '\"') i++;
					endOfString = str[i] == '\"';
					str[i] = 0;
					key = strchr(envpGet, keyStart);
					if (key) {
						char* val = strchr(key, '=') + 1;
						strncpy(outStr + j, val, strlen(val));
						j += strlen(val);
					}
					if (endOfString) {
						outStr[j] = '\"';
						i++;
						j++;
						break;
					}
				}
				outStr[j] = str[i];
				i++;
				j++;
			}
			if (str[i]) {
				outStr[j] = str[i];
				i++;
				j++;
			}
			continue;
		}

		if (str[i] == '$') {
			keyStart = ++i;
			while (str[i] && str[i] != ' ' && str[i] != '\"' && str[i] != '\''){
				i++;
			}
			str[i] = 0;
			key = strchr(envpGet, keyStart);

			if (key) {
				char* val = strchr(key, '=') + 1;
				strncpy(outStr + j, val, strlen(val));
				j += strlen(val);
			}
			continue;
		}

		outStr[j] = str[i];
		i++;
		j++;
	}

	outStr[j] = 0;

	return outStr;
}
*/