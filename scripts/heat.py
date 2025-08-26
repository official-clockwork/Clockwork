import numpy as np
import matplotlib.pyplot as plt
import os

def S(a,b):
    return (a,b)

def su(a,b):
    return (a[0]+b[0],a[1]+b[1])

mat = [S(203,430),S(853,849), S(937,876), S(1086,1542), S(2825,2709)]

# --- Raw PSQT data as (MG, EG) tuples ---
pawn = [
S(269,297),     S(171,375),     S(344,262),     S(217,237),     S(272,269),     S(116,331),     S(62,397),      S(156,355),
S(-55,178),     S(10,163),      S(91,116),      S(148,97),      S(190,87),      S(115,88),      S(48,161),      S(-18,125),
S(-38,32),      S(1,27),        S(4,-12),       S(82,-38),      S(78,-48),      S(22,-33),      S(31,-1),       S(-37,16),
S(-88,0),       S(4,-28),       S(-7,-69),      S(54,-88),      S(50,-82),      S(10,-92),      S(31,-34),      S(-83,-33),
S(-67,-53),     S(-21,-54),     S(-27,-81),     S(11,-75),      S(26,-73),      S(31,-79),      S(22,-56),      S(-64,-61),
S(-74,-26),     S(-26,-37),     S(-47,-59),     S(-35,-65),     S(7,-83),       S(1,-59),       S(20,-60),      S(-65,-26),

]

pawn = [su(mat[0],p) for p in pawn]

knight = [

S(-307,-121),   S(-297,120),    S(27,32),       S(40,108),      S(256,-31),     S(-288,127),    S(-58,-2),      S(-264,-255),
S(-26,-59),     S(52,27),       S(54,31),       S(186,61),      S(82,51),       S(169,4),       S(83,32),       S(53,-17),
S(8,34),        S(92,46),       S(131,70),      S(246,6),       S(268,5),       S(210,16),      S(130,37),      S(47,20),
S(24,3),        S(35,38),       S(93,53),       S(93,38),       S(39,123),      S(137,57),      S(48,63),       S(67,18),
S(-18,10),      S(18,0),        S(34,53),       S(35,71),       S(41,46),       S(16,62),       S(42,26),       S(27,20),
S(-79,-67),     S(-43,7),       S(-33,1),       S(48,28),       S(29,10),       S(-28,-10),     S(-10,23),      S(-48,-71),
S(-118,-93),    S(-51,-21),     S(-48,-23),     S(-4,-38),      S(16,-13),      S(-9,-8),       S(-36,-17),     S(-106,-25),
S(-218,-43),    S(-25,-64),     S(-82,-38),     S(-49,-25),     S(-49,-41),     S(-30,-100),    S(-9,-90),      S(-170,-78),

]
knight = [su(mat[1],p) for p in knight]

bishop = [

S(-79,93),      S(-95,94),      S(-100,47),     S(-116,25),     S(-123,63),     S(-144,70),     S(-23,75),      S(-74,79),
S(-73,2),       S(-33,53),      S(33,54),       S(31,19),       S(1,57),        S(-3,56),       S(-51,67),      S(-64,52),
S(-42,38),      S(97,11),       S(37,39),       S(157,4),       S(105,16),      S(137,41),      S(63,51),       S(82,5),
S(1,-35),       S(11,38),       S(73,21),       S(87,40),       S(85,22),       S(49,20),       S(0,56),        S(35,14),
S(12,17),       S(35,-18),      S(-18,25),      S(78,-4),       S(30,2),        S(-24,55),      S(34,4),        S(26,-40),
S(4,-16),       S(-23,-8),      S(4,2), S(-9,22),       S(-6,10),       S(5,8), S(-19,-24),     S(-16,-20),
S(0,-52),       S(5,-22),       S(-39,-39),     S(-10,-17),     S(-18,8),       S(-4,-11),      S(7,-10),       S(-19,-78),
S(-112,-11),    S(-72,-2),      S(5,-52),       S(-68,-19),     S(-118,-4),     S(-39,-26),     S(-18,-42),     S(-100,1),

]
bishop = [su(mat[2],p) for p in bishop]

rook = [

S(285,15),      S(331,13),      S(299,-8),      S(258,19),      S(245,37),      S(335,7),       S(282,6),       S(386,-41),
S(240,20),      S(155,51),      S(281,7),       S(338,14),      S(320,7),       S(282,-19),     S(182,53),      S(379,-43),
S(141,60),      S(198,22),      S(229,51),      S(283,32),      S(355,-7),      S(304,-17),     S(298,-13),     S(235,9),
S(8,43),        S(24,43),       S(114,13),      S(89,31),       S(139,-4),      S(135,32),      S(87,34),       S(102,2),
S(-63,45),      S(-45,41),      S(-23,52),      S(20,21),       S(24,34),       S(10,38),       S(-18,3),       S(-1,15),
S(-131,7),      S(-81,6),       S(-50,7),       S(-9,8),        S(-10,-18),     S(-19,-11),     S(-20,-41),     S(-60,3),
S(-145,24),     S(-93,7),       S(-32,-1),      S(10,-24),      S(-22,3),       S(-14,-1),      S(-50,-9),      S(-123,-4),
S(-81,11),      S(-42,8),       S(-19,11),      S(9,0), S(30,-28),      S(-40,25),      S(-16,-13),     S(-74,0),

]
rook = [su(mat[3],p) for p in rook]

queen = [

S(39,41),       S(202,32),      S(184,63),      S(159,148),     S(270,31),      S(313,-11),     S(260,53),      S(174,61),
S(26,58),       S(-79,184),     S(-22,271),     S(98,227),      S(54,231),      S(116,217),     S(4,175),       S(151,109),
S(-26,91),      S(-1,111),      S(-36,100),     S(137,106),     S(118,164),     S(159,154),     S(144,113),     S(125,138),
S(-52,66),      S(-55,138),     S(-19,163),     S(-30,230),     S(-11,221),     S(39,172),      S(-5,191),      S(27,174),
S(-23,-13),     S(-48,82),      S(-69,86),      S(-68,149),     S(-35,140),     S(-28,107),     S(-7,91),       S(-42,105),
S(-39,-43),     S(-19,-78),     S(-41,-57),     S(-69,-47),     S(-13,-40),     S(-34,33),      S(-13,-7),      S(7,-60),
S(1,-99),       S(-46,-162),    S(18,-208),     S(12,-187),     S(20,-209),     S(42,-170),     S(7,-244),      S(46,-66),
S(-43,-71),     S(10,-253),     S(27,-242),     S(30,-260),     S(-12,-350),    S(-36,-205),    S(-9,-264),     S(-30,-54),

]
queen = [su(mat[4],p) for p in queen]

king = [

S(814,-280),    S(443,-52),     S(366,-25),     S(524,-43),     S(465,49),      S(356,-8),      S(309,-6),      S(67,-105),
S(-11,-18),     S(-23,81),      S(-10,96),      S(89,42),       S(-40,67),      S(36,117),      S(-106,144),    S(32,26),
S(-204,144),    S(-164,137),    S(-145,132),    S(-142,97),     S(-185,156),    S(-88,167),     S(-105,149),    S(-82,78),
S(-252,75),     S(-88,132),     S(-209,122),    S(-171,104),    S(-241,105),    S(-179,114),    S(-132,114),    S(-326,102),
S(-130,19),     S(-106,32),     S(-179,63),     S(-187,74),     S(-208,51),     S(-235,89),     S(-143,55),     S(-259,72),
S(-62,-41),     S(-52,-20),     S(-172,18),     S(-199,41),     S(-184,36),     S(-185,26),     S(-79,-13),     S(-62,-28),
S(0,-88),       S(11,-71),      S(-40,-66),     S(-96,-17),     S(-78,-20),     S(-27,-48),     S(42,-92),      S(102,-102),
S(-30,-68),     S(121,-128),    S(76,-98),      S(-57,-79),     S(38,-94),      S(21,-90),      S(175,-150),    S(160,-182),

]

# --- Helper functions ---
def pad_pawn_table(table):
    """Pad pawn table to 64 by adding top and bottom rows of zeros"""
    top = [(0,0)]*8
    bottom = [(0,0)]*8
    return top + table + bottom

def tuples_to_arrays(table):
    """Convert list of tuples to MG and EG 8x8 arrays"""
    mg = np.array([t[0] for t in table]).reshape(8,8)
    eg = np.array([t[1] for t in table]).reshape(8,8)
    return mg, eg

def save_combined_heatmap(tables, titles, filename="all_heatmaps_dark.png"):
    fig, axes = plt.subplots(2, len(tables), figsize=(4*len(tables), 8))
    
    # Set dark figure background
    fig.patch.set_facecolor('#1e1e1e')
    
    for i, table in enumerate(tables):
        mg, eg = tuples_to_arrays(table)
        
        # MG heatmap
        ax_mg = axes[0, i]
        im_mg = ax_mg.imshow(mg, cmap='coolwarm', origin='upper')
        ax_mg.set_title(f"{titles[i]} MG", color='white')
        ax_mg.set_facecolor('#1e1e1e')
        ax_mg.tick_params(colors='white')
        fig.colorbar(im_mg, ax=ax_mg, orientation='vertical', shrink=0.8)
        
        # EG heatmap
        ax_eg = axes[1, i]
        im_eg = ax_eg.imshow(eg, cmap='coolwarm', origin='upper')
        ax_eg.set_title(f"{titles[i]} EG", color='white')
        ax_eg.set_facecolor('#1e1e1e')
        ax_eg.tick_params(colors='white')
        fig.colorbar(im_eg, ax=ax_eg, orientation='vertical', shrink=0.8)
    
    plt.tight_layout()
    plt.savefig(filename, facecolor=fig.get_facecolor())
    plt.close()
    print(f"Saved combined dark heatmap to '{filename}'.")

def plot_heatmaps(tables, titles):
    """Plot MG and EG heatmaps side by side"""
    fig, axes = plt.subplots(2, len(tables), figsize=(4*len(tables),8))
    for i, table in enumerate(tables):
        mg, eg = tuples_to_arrays(table)
        im_mg = axes[0, i].imshow(mg, cmap='coolwarm', origin='upper')
        im_eg = axes[1, i].imshow(eg, cmap='coolwarm', origin='upper')
        axes[0, i].set_title(f"{titles[i]} MG")
        axes[1, i].set_title(f"{titles[i]} EG")
        for ax, im in zip([axes[0,i], axes[1,i]], [im_mg, im_eg]):
            plt.colorbar(im, ax=ax)
    plt.tight_layout()
    plt.show()

# --- Prepare tables ---
pawn_padded = pad_pawn_table(pawn)
tables = [pawn_padded, knight, bishop, rook, queen, king]
titles = ['Pawn', 'Knight', 'Bishop', 'Rook', 'Queen', 'King']


save_combined_heatmap(tables, titles)

# --- Plot heatmaps ---
plot_heatmaps(tables, titles)
