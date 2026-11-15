"""Run as an Unreal Python commandlet against the intended feature worktree."""
import os
import sys
import unreal
sys.path.insert(0, os.path.join(unreal.Paths.project_dir(), 'Content', 'Python'))
import ft_powerups
ft_powerups.build()
