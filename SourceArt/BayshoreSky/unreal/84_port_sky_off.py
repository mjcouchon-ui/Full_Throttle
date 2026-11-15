"""Run in the port's editor: takes the sky back out of L_Bayshore (the level's own NightSky, Sun and SkyLight and ft_cel's
fog come back), for A/B checks against the original Level 1. 80_port_apply.py puts it back."""
import unreal
import ft_sky
ft_sky.remove(0)
print("BSKY port sky removed (A/B baseline)")
