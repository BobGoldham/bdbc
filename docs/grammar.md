### Warning
GitHub does not correctly render LaTeX embedded into Markdown files.
Expect issues with rendering if you are viewing this on GitHub.
For a better viewing experience, [download this file](https://raw.githubusercontent.com/BobGoldham/bdbc/master/docs/grammar.md).

$$
\begin{align}
[S] &\to \begin{cases}
    \epsilon\\
    [exit]\\
    [var]S
\end{cases}
\\
[var] &\to \text{var var }[varname] \text{=} [expr][endline]
\\
[exit] &\to \text{exit } [expr][endline]
\\
[expr] &\to \begin{cases}
    [int\_lit]\\
    [ident]
\end{cases}
\\
[int\_lit] &\to 1|2|3|...
\\
[endline] &\to \begin{cases}
    \text{?}\\
    [exclamate]
\end{cases}
\\
[exclamate] &\to \begin{cases}
    [exclamate]\text{!}\\
    \text{!}
\end{cases}
\end{align}
$$

# TODO

Parsing for [endline] and its children is not yet implemented.