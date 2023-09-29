### Warning
GitHub does not correctly render LaTeX embedded into Markdown files.
Expect issues with rendering if you are viewing this on GitHub.
For a better viewing experience, [download this file](https://raw.githubusercontent.com/BobGoldham/bdbc/master/docs/grammar.md).

$$
\begin{align}
[S] &\to [exit]
\\
[exit] &\to \text{exit } [expr][endline]
\\
[expr] &\to [int\_lit]
\\
[int\_lit] &\to 1|2|3|...
\\
[endline] &\to \text{?}|[exclamate]
\\
[exclamate] &\to [exclamate]\text{!}|\text{!}
\end{align}
$$

# TODO

Parsing for [endline] and its children is not yet implemented.