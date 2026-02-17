# Junior Developer Workflow Guide

Welcome to the team! This guide will walk you through how we work with GitHub. Follow these steps for every task, and you'll never go wrong.

---

## Our Branch Structure

main (PRODUCTION! - dont touch) -> dev (DEVELOPMENT - where your code goes) -> #[number]-[description]-[name] (YOUR branches)

|        Branch         | Who Can Push  | Who Can Merge |                         Purpose                         |
|-----------------------|---------------|---------------|---------------------------------------------------------|
|        `main`         |   Leads ONLY  |  Leads ONLY   |         Live production code. NEVER push here.          |
|        `dev`          |    Everyone   |  Leads ONLY   |     Latest development code. Always branch from here.   |
| Your feature branches |      You      |  Leads ONLY   | Where you do your work. Named like `#5-add-login-sarah` |

---

## Step-by-Step Workflow

### **Step 1: You will be assigned a task**

1. Go to our **Projects** tab and open the Development Board
2. Find the card you were assigned in the **"To Do"** column
3. Click on the card to see details
4. Assign yourself to it (click "Assignees" and add yourself)
5. Drag the card to **"In Progress"**

### **Step 2: Set Up Your Local Environment**

```bash
# First time only - clone the repository
git clone https://github.com/your-org/your-repo.git
cd your-repo

# Always start fresh
git checkout dev
git pull origin dev

```

### **Step 3: Create your branch**

```bash
# IMPORTANT: Always branch FROM dev
git checkout dev
git pull origin dev  # Get latest changes

# Create your branch with our naming convention
# Format: #[card-number]-[short-description]-[your-name]
git checkout -b #5-add-login-sarah dev

# Example branches:
# #5-add-login-sarah
# #7-fix-navbar-mike
# #12-update-api-emma

```

### **Step 4: Work on your task**

```bash
# Make changes to your code
# Save your files

# Check what files you've changed
git status

# Add files to commit
git add filename.js           # Add specific file
git add .                     # Add all changed files

# Commit with a clear message
git commit -m "Add login form UI"
git commit -m "Implement validation logic"
git commit -m "Fix styling issues"

# Push to GitHub regularly
git push origin #5-add-login-sarah
```

### **Step 5: Open a pull request**

Once your feature is complete:

1. **Go to GitHub** and you'll see a yellow banner saying "#5-add-login-sarah had recent pushes"

2. Click **"Compare & pull request"**

3. **⚠️ CRITICAL: Check the branches!**
   - **Base:** `dev` (NOT `main`!)
   - **Compare:** `#5-add-login-sarah`

4. **Fill out the PR template completely**
- Add the card number (e.g., `Closes #5`)
- Describe what you changed
- Explain how to test
- Check off the self-review items

5. **Assign reviewers**
- Click "Reviewers" on the right sidebar
- Add both managers (or at least one)
- Example: `@tirthp14` and `@artin59`

6. **Click "Create pull request"**

### **Step 6: The Review Process**

#### **Wait for Leads to Review**
- Give the leads some grace time to review your code
- You'll get an email notification when feedback is posted
- Check back regularly for comments

#### **If Changes Are Requested:**

# 1. Make the requested changes locally
# Edit your files...

# 2. Commit the fixes
```bash
git add .
git commit -m "Fix: address review feedback"
```
# 3. Push to the SAME branch
```bash
git push origin #5-add-login-sarah
```

### **Step 7: Clean up**

```bash
# Switch back to dev
git checkout dev
git pull origin dev  # Get latest including your merged code
```

---

## Branch Naming Convention

### MUST FOLLOW THIS FORMAT:

```bash
#[card-number]-[short-description]-[your-name]
```
eg. #5-add-login-sarah, #12-fix-navbar-mike, #23-update-api-emma

- #5 → Links to the card in our project board
- add-login → Tells us what you're working on
- sarah → Tells us who owns the branch

---

## Git Cheat Sheet

```bash
# Start a new feature
git checkout dev
git pull origin dev
git checkout -b #5-description-name dev

# Daily work
git status                    # See what's changed
git add .                     # Stage all changes
git commit -m "message"       # Commit with message
git push origin branch-name   # Push to GitHub

# Update your branch with latest dev
git checkout dev
git pull origin dev
git checkout #5-description-name
git merge dev                 # Merge latest dev into your branch
git push origin #5-description-name

# Fix a mistake
git restore filename          # Undo uncommitted changes
git reset HEAD~1              # Undo last commit (keep changes)
git reset --hard HEAD~1       # Undo last commit (delete changes)

# After PR is merged
git checkout dev
git pull origin dev
```
