# 🐚 minishell
A simple Unix shell clone written in C.
Supports built-in commands, redirections, pipes, and signal handling.

# 🔄 General Git Commands
Action					|	Command
-----------------------------------------------------------------------
Add files or changes	|	git add .

Commit changes			|	git commit -m "your message"

Pull latest changes		|	git pull origin main

View commit history		|	git log

Solve merge conflicts	|	git mergetool or manual fix

# 🌿 Working with Branches
Action					|	Command
-----------------------------------------------------------------------
Create a new branch		|	git checkout -b feature/feature_name

Switch to a branch		|	git checkout feature/feature_name

Push branch to GitHub	|	git push origin feature/feature_name

Delete local branch		|	git branch -d feature/feature_name

Delete remote branch	|	git push origin --delete feature/feature_name

See local branches		|	git branch

Pull a local main copy	|	git checkout -b main origin/main

# 🛟 Useful Tips
Use Case								|	Command
------------------------------------------------------------------------
Temporarily save uncommitted changes	|	git stash

Apply stashed changes					|	git stash apply

Check stash list						|	git stash list

Resolve merge conflicts manually		|	Open the conflicted file and fix the sections marked by Git (<<<<<<<)

# 📌 Commit Categories

Use these keywords at the start of your commit messages:

- ✨ feat:     Add a new feature (e.g. feat: add pipe support)
- 🐛 fix:      Fix a bug (e.g. fix: handle empty input crash)
- 🛠️ refactor: Refactor code without changing behavior (e.g. refactor: simplify parser logic)
- 🎨 style:    Improve formatting, indentation, etc. (no logic changes)
- 📚 docs:     Add or update documentation (README, comments, etc.)
- ✅ test:     Add or update tests
- 🧪 ci:       Add or update CI files (e.g. GitHub Actions)
- 📦 build:    Changes to build system or dependencies (Makefile, etc.)
- 🚧 chore:    Routine tasks or maintenance (e.g. chore: update .gitignore)

# 💡 Typical Workflow
1. Switch to main and pull the latest changes from GitHub

	git checkout main

	git pull origin main

2. Create a new branch from updated main

	git checkout -b feature/new-builtin

3. Work on the branch, then push it

	git add .

	git commit -m "feat: Add new builtin command"

	git push origin feature/new-builtin

4. Open a PR on GitHub and merge once approved

5. Switch back to main and pull merged changes

	git checkout main

	git pull origin main
