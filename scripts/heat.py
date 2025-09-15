import numpy as np
import matplotlib.pyplot as plt
import os

def S(a,b):
    return (a,b)

def su(a,b):
    return (a[0]+b[0],a[1]+b[1])

mat = [S(195,193),S(766,633),S(787,620),S(1150,1047),S(3360,997)]

# --- Raw PSQT data as (MG, EG) tuples ---
pawn = [
S(156,533),     S(111,636),     S(406,532),     S(371,444),     S(371,386),     S(469,426),     S(294,518),     S(339,511),
S(103,358),     S(251,361),     S(200,300),     S(195,279),     S(173,263),     S(152,272),     S(79,325),      S(-31,358),
S(58,217),      S(107,206),     S(86,164),      S(96,137),      S(84,142),      S(20,159),      S(6,208),       S(-57,249),
S(50,152),      S(128,151),     S(71,116),      S(76,109),      S(64,103),      S(9,121),       S(-10,174),     S(-80,188),
S(99,118),      S(194,112),     S(84,137),      S(37,131),      S(26,124),      S(-18,127),     S(-23,163),     S(-75,165),
S(64,125),      S(225,113),     S(113,141),     S(-3,160),      S(-34,122),     S(-43,147),     S(-44,181),     S(-92,190),
]

pawn = [su(mat[0],p) for p in pawn]

knight = [
S(-315,69),     S(-24,129),     S(-267,252),    S(223,95),      S(0,188),       S(-48,197),     S(-182,142),    S(-397,138),
S(94,35),       S(117,150),     S(320,78),      S(206,169),     S(252,179),     S(179,103),     S(110,106),     S(84,57),
S(146,144),     S(276,163),     S(349,145),     S(339,122),     S(278,143),     S(211,152),     S(184,126),     S(100,96),
S(222,102),     S(147,169),     S(229,158),     S(164,178),     S(169,191),     S(162,177),     S(117,141),     S(112,92),
S(103,92),      S(162,122),     S(114,162),     S(143,162),     S(97,180),      S(122,171),     S(120,117),     S(59,57),
S(25,44),       S(112,81),      S(92,72),       S(118,130),     S(115,133),     S(35,101),      S(46,87),       S(6,19),
S(74,55),       S(57,105),      S(77,76),       S(73,85),       S(80,67),       S(45,48),       S(13,72),       S(-6,-48),
S(-121,127),    S(42,-38),      S(29,28),       S(78,29),       S(14,53),       S(7,-11),       S(22,-56),      S(-164,-22),
]
knight = [su(mat[1],p) for p in knight]

bishop = [
S(-18,203),     S(5,204),       S(-6,177),      S(-42,197),     S(-91,223),     S(-164,239),    S(-43,252),     S(20,229),
S(22,161),      S(3,228),       S(108,192),     S(115,184),     S(7,237),       S(78,207),      S(70,194),      S(13,179),
S(129,202),     S(243,182),     S(358,150),     S(229,161),     S(185,161),     S(156,190),     S(177,185),     S(110,165),
S(116,129),     S(94,199),      S(183,169),     S(168,174),     S(194,168),     S(135,184),     S(79,217),      S(66,168),
S(117,117),     S(124,156),     S(72,198),      S(148,162),     S(134,192),     S(63,211),      S(95,160),      S(89,119),
S(132,132),     S(113,149),     S(122,137),     S(78,190),      S(77,191),      S(71,185),      S(105,124),     S(75,124),
S(111,65),      S(168,111),     S(132,115),     S(83,138),      S(53,136),      S(110,107),     S(82,113),      S(123,47),
S(60,86),       S(-54,184),     S(17,126),      S(36,121),      S(25,109),      S(63,66),       S(65,93),       S(28,124),
]
bishop = [su(mat[2],p) for p in bishop]

rook = [
S(464,289),     S(432,329),     S(239,415),     S(270,394),     S(324,343),     S(332,330),     S(297,338),     S(269,351),
S(374,337),     S(319,363),     S(423,354),     S(346,383),     S(392,343),     S(302,363),     S(229,370),     S(231,372),
S(197,402),     S(328,370),     S(351,369),     S(313,369),     S(251,362),     S(185,387),     S(226,365),     S(137,401),
S(119,378),     S(147,378),     S(159,385),     S(178,350),     S(175,353),     S(118,384),     S(109,370),     S(72,373),
S(-7,347),      S(92,334),      S(47,371),      S(71,355),      S(51,360),      S(13,392),      S(0,386),       S(-30,380),
S(-6,320),      S(109,277),     S(53,297),      S(42,298),      S(43,308),      S(-3,335),      S(8,308),       S(-32,318),
S(27,296),      S(93,267),      S(83,275),      S(63,286),      S(67,280),      S(45,287),      S(8,277),       S(-37,278),
S(23,279),      S(47,326),      S(87,307),      S(127,271),     S(108,291),     S(86,299),      S(68,276),      S(40,289),
]
rook = [su(mat[3],p) for p in rook]

queen = [
S(378,940),     S(390,936),     S(415,985),     S(318,1052),    S(305,1009),    S(218,1053),    S(239,1013),    S(229,944),
S(364,959),     S(164,1116),    S(336,1068),    S(186,1171),    S(190,1113),    S(179,1071),    S(136,1016),    S(252,901),
S(342,1011),    S(410,1015),    S(380,1025),    S(283,1072),    S(196,1092),    S(195,1010),    S(239,931),     S(259,879),
S(307,965),     S(232,1056),    S(228,1021),    S(174,1051),    S(150,1053),    S(188,958),     S(180,932),     S(239,853),
S(220,972),     S(256,900),     S(183,1000),    S(195,916),     S(158,976),     S(162,910),     S(184,864),     S(206,805),
S(245,804),     S(261,802),     S(226,860),     S(196,819),     S(196,777),     S(170,836),     S(213,740),     S(222,777),
S(281,558),     S(295,474),     S(286,598),     S(270,660),     S(244,683),     S(263,608),     S(237,698),     S(243,708),
S(349,302),     S(235,295),     S(211,500),     S(302,502),     S(297,604),     S(300,521),     S(259,631),     S(218,741),
]
queen = [su(mat[4],p) for p in queen]

king = [
S(593,-481),    S(339,1),       S(190,70),      S(-130,31),     S(0,0), S(0,0), S(0,0), S(0,0),
S(364,-102),    S(74,93),       S(5,87),        S(288,37),      S(0,0), S(0,0), S(0,0), S(0,0),
S(157,32),      S(-19,68),      S(63,64),       S(-68,56),      S(0,0), S(0,0), S(0,0), S(0,0),
S(72,0),        S(120,30),      S(89,38),       S(-8,51),       S(0,0), S(0,0), S(0,0), S(0,0),
S(-78,-14),     S(-39,10),      S(-26,36),      S(-81,57),      S(0,0), S(0,0), S(0,0), S(0,0),
S(-116,-24),    S(-55,-22),     S(-156,28),     S(-172,53),     S(0,0), S(0,0), S(0,0), S(0,0),
S(70,-88),      S(18,-71),      S(-81,-23),     S(-164,1),      S(0,0), S(0,0), S(0,0), S(0,0),
S(142,-181),    S(133,-132),    S(6,-95),       S(72,-157),     S(0,0), S(0,0), S(0,0), S(0,0),

]

# --- Helper functions ---
def pad_pawn_table(table):
    """Pad pawn table to 64 by adding top and bottom rows of zeros"""
    top = [(0,0)]*8
    bottom = [(0,0)]*8
    return top + table + bottom

def tuples_to_arrays(table):
    """Convert list of tuples to MG and EG arrays (auto reshape)."""
    n = len(table)
    if n % 8 != 0:
        raise ValueError(f"Table length {n} is not divisible by 8")
    rows = n // 8
    mg = np.array([t[0] for t in table]).reshape(rows, 8)
    eg = np.array([t[1] for t in table]).reshape(rows, 8)
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

def normalize_and_print(table, title, ignore_padding=False):
    """Subtract mean from MG and EG values, print adjusted table and mean."""
    
    # If pawn, ignore padding for mean computation
    if ignore_padding:
        # Remove top and bottom 8 entries (padding rows)
        core_table = table[8:-8]
    else:
        core_table = table
    
    mg, eg = tuples_to_arrays(core_table)
    mean_mg = np.mean(mg)
    mean_eg = np.mean(eg)
    
    adjusted = []
    for (m, e) in table:
        adj_m = int(round(m - mean_mg))
        adj_e = int(round(e - mean_eg))
        adjusted.append(S(adj_m, adj_e))
    
    # Print title
    print(f"\n--- {title} ---")
    # Print adjusted table in rows of 8
    for i in range(0, len(adjusted), 8):
        row = adjusted[i:i+8]
        print(",\t".join(f"S({m},{e})" for (m,e) in row) + ",")
    
    # Print the mean (based only on non-padded data if ignore_padding=True)
    print(f"Mean: S({int(round(mean_mg))},{int(round(mean_eg))})")


def print_normalized_tables(tables, titles):
    """Process all tables and print normalized versions with means."""
    for table, title in zip(tables, titles):
        ignore_padding = (title == "Pawn")
        normalize_and_print(table, title, ignore_padding=ignore_padding)


# --- Prepare tables ---
pawn_padded = pad_pawn_table(pawn)
tables = [pawn_padded, knight, bishop, rook, queen, king]
titles = ['Pawn', 'Knight', 'Bishop', 'Rook', 'Queen', 'King']


save_combined_heatmap(tables, titles)

# --- Plot heatmaps ---
plot_heatmaps(tables, titles)

print_normalized_tables(tables, titles)

