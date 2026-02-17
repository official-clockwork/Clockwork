#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(136, 203);
inline const PParam KNIGHT_MAT = S(575, 625);
inline const PParam BISHOP_MAT = S(604, 646);
inline const PParam ROOK_MAT   = S(466, 681);
inline const PParam QUEEN_MAT  = S(1089, 1086);
inline const PParam TEMPO_VAL  = S(57, 21);

inline const PParam BISHOP_PAIR_VAL   = S(54, 194);
inline const PParam ROOK_OPEN_VAL     = S(105, -11);
inline const PParam ROOK_SEMIOPEN_VAL = S(43, 22);
inline const PParam MINOR_BEHIND_PAWN = S(10, 41);

inline const PParam DOUBLED_PAWN_VAL = S(-7, -73);
inline const PParam ISOLATED_PAWN_VAL = S(-19, -16);

inline const PParam POTENTIAL_CHECKER_VAL = S(-49, -24);
inline const PParam OUTPOST_KNIGHT_VAL    = S(36, 65);
inline const PParam OUTPOST_BISHOP_VAL    = S(51, 42);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(41, 6);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(52, -23);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(26, 54);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(67, -38);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(16, -0), S(38, 24), S(57, 48), S(133, 144), S(378, 202), S(454, 692),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(54, 34), S(44, 27), S(53, 55), S(115, 142), S(399, 95),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-71, -123), S(-68, -100), S(-47, 11), S(8, 117), S(91, 266), S(263, 379),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(36, -44), S(34, -4), S(22, 32), S(1, 110), S(45, 223), S(195, 289),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(22, -31), S(3, 20), S(3, -12), S(2, -42), S(-5, -116), S(-212, -222),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-5, 148), S(-16, 113), S(-11, 44), S(-4, 8), S(1, 7), S(37, 6), S(10, 1),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-310, -58), S(-41, 28), S(-24, 60), S(12, 97), S(31, 114), S(48, 115), S(31, 107),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(60, 12), S(130, 177), S(164, 266), S(191, 301), S(230, 318), S(254, 350), S(286, 346), S(316, 359), S(368, 273),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(86, -3), S(130, 171), S(185, 237), S(212, 282), S(236, 314), S(253, 338), S(260, 357), S(274, 367), S(280, 384), S(305, 366), S(315, 368), S(363, 317), S(360, 333), S(393, 278),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(311, 246), S(229, 421), S(254, 447), S(271, 458), S(282, 470), S(288, 481), S(294, 491), S(303, 493), S(308, 504), S(319, 505), S(331, 506), S(338, 511), S(344, 510), S(360, 482), S(444, 380),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(494, 522), S(637, 536), S(662, 647), S(680, 760), S(696, 806), S(704, 851), S(710, 882), S(718, 893), S(721, 917), S(724, 931), S(730, 938), S(734, 946), S(742, 940), S(744, 945), S(750, 940), S(748, 943), S(750, 939), S(755, 934), S(764, 922), S(772, 918), S(781, 893), S(803, 853), S(802, 850), S(798, 799), S(786, 785), S(781, 752), S(712, 812), S(736, 718),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(490, -181), S(109, -106), S(29, -24), S(15, 9), S(-5, 9), S(-20, 1), S(-26, 9), S(-42, 16), S(-29, -25),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(12, -8), S(15, -15), S(13, -12), S(6, -8), S(2, 195),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(4, -0), S(7, -11), S(6, -9), S(5, -5), S(6, 394),
};

inline const PParam PAWN_THREAT_KNIGHT = S(211, 92);
inline const PParam PAWN_THREAT_BISHOP = S(189, 141);
inline const PParam PAWN_THREAT_ROOK   = S(190, 126);
inline const PParam PAWN_THREAT_QUEEN  = S(166, -14);

inline const PParam KNIGHT_THREAT_BISHOP = S(108, 99);
inline const PParam KNIGHT_THREAT_ROOK   = S(225, 71);
inline const PParam KNIGHT_THREAT_QUEEN  = S(150, 2);

inline const PParam BISHOP_THREAT_KNIGHT = S(105, 58);
inline const PParam BISHOP_THREAT_ROOK   = S(211, 126);
inline const PParam BISHOP_THREAT_QUEEN  = S(175, 103);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(10, -33), S(-3, -6), S(-5, -14), S(-8, -23), S(-12, -31), S(-16, -35), S(-18, -46), S(-25, -43), S(-36, -43),
};

inline const PParam ROOK_LINEUP = S(12, 65);

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(298, 343),    S(150, 444),    S(291, 415),    S(298, 283),    S(321, 238),    S(215, 323),    S(158, 356),    S(265, 308),    //
    S(118, 208),    S(149, 249),    S(188, 165),    S(173, 104),    S(154, 86),     S(126, 138),    S(100, 193),    S(49, 220),     //
    S(82, 173),     S(71, 176),     S(113, 101),    S(113, 81),     S(101, 78),     S(74, 110),     S(26, 159),     S(12, 187),     //
    S(57, 125),     S(48, 149),     S(82, 96),      S(80, 89),      S(58, 93),      S(40, 117),     S(-19, 161),    S(-20, 152),    //
    S(55, 100),     S(93, 107),     S(76, 136),     S(70, 123),     S(42, 119),     S(20, 126),     S(-14, 144),    S(-22, 137),    //
    S(67, 101),     S(156, 118),    S(140, 173),    S(91, 155),     S(62, 143),     S(41, 137),     S(13, 149),     S(-5, 150),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-255, -76),   S(-147, 188),   S(-99, 49),     S(103, 176),    S(32, 188),     S(-111, 189),   S(-246, 239),   S(-292, -5),    //
    S(73, 164),     S(116, 190),    S(207, 149),    S(202, 160),    S(196, 168),    S(111, 196),    S(75, 205),     S(47, 184),     //
    S(153, 149),    S(199, 149),    S(205, 208),    S(209, 198),    S(168, 234),    S(108, 244),    S(114, 186),    S(78, 176),     //
    S(224, 171),    S(232, 191),    S(231, 221),    S(216, 253),    S(217, 263),    S(174, 245),    S(164, 209),    S(159, 183),    //
    S(207, 176),    S(243, 163),    S(212, 221),    S(207, 240),    S(184, 244),    S(183, 230),    S(180, 178),    S(143, 180),    //
    S(154, 142),    S(179, 159),    S(168, 198),    S(175, 223),    S(172, 221),    S(136, 203),    S(127, 170),    S(99, 132),     //
    S(160, 147),    S(181, 140),    S(150, 155),    S(153, 182),    S(146, 181),    S(118, 136),    S(104, 151),    S(94, 65),      //
    S(93, 88),      S(141, 160),    S(155, 127),    S(173, 129),    S(154, 152),    S(122, 124),    S(106, 141),    S(67, 52),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-3, 297),     S(-17, 308),    S(-272, 346),   S(-144, 289),   S(-151, 307),   S(-162, 303),   S(-50, 303),    S(28, 276),     //
    S(117, 212),    S(62, 291),     S(94, 244),     S(44, 260),     S(58, 261),     S(81, 262),     S(121, 257),    S(94, 236),     //
    S(183, 226),    S(195, 246),    S(192, 252),    S(171, 256),    S(143, 252),    S(151, 255),    S(163, 245),    S(157, 216),    //
    S(157, 222),    S(191, 229),    S(194, 253),    S(191, 292),    S(221, 283),    S(152, 257),    S(164, 219),    S(124, 221),    //
    S(187, 177),    S(204, 214),    S(209, 238),    S(211, 266),    S(187, 281),    S(174, 259),    S(143, 234),    S(138, 190),    //
    S(213, 185),    S(248, 205),    S(256, 229),    S(201, 256),    S(196, 233),    S(191, 245),    S(200, 224),    S(140, 215),    //
    S(175, 151),    S(273, 169),    S(231, 191),    S(199, 212),    S(181, 216),    S(184, 190),    S(184, 181),    S(180, 174),    //
    S(208, 157),    S(195, 179),    S(192, 211),    S(209, 183),    S(199, 194),    S(198, 233),    S(196, 213),    S(186, 179),    //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(430, 389),    S(444, 406),    S(420, 421),    S(439, 383),    S(421, 387),    S(385, 400),    S(381, 416),    S(374, 419),    //
    S(306, 472),    S(382, 469),    S(457, 435),    S(405, 439),    S(391, 447),    S(370, 452),    S(304, 482),    S(286, 489),    //
    S(277, 466),    S(376, 454),    S(435, 415),    S(416, 400),    S(366, 431),    S(325, 454),    S(329, 449),    S(262, 491),    //
    S(281, 424),    S(355, 437),    S(393, 408),    S(367, 412),    S(358, 423),    S(319, 444),    S(315, 439),    S(240, 468),    //
    S(263, 357),    S(327, 374),    S(315, 396),    S(295, 394),    S(272, 418),    S(262, 429),    S(231, 425),    S(212, 421),    //
    S(256, 301),    S(316, 328),    S(302, 354),    S(288, 352),    S(275, 361),    S(249, 384),    S(247, 359),    S(213, 366),    //
    S(148, 312),    S(278, 279),    S(289, 309),    S(279, 332),    S(264, 337),    S(257, 340),    S(239, 327),    S(215, 338),    //
    S(212, 299),    S(235, 319),    S(291, 309),    S(304, 306),    S(293, 314),    S(273, 329),    S(262, 325),    S(242, 334),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(554, 643),    S(606, 593),    S(526, 723),    S(493, 793),    S(478, 777),    S(515, 715),    S(535, 641),    S(490, 673),    //
    S(552, 700),    S(515, 799),    S(506, 831),    S(362, 927),    S(368, 916),    S(433, 862),    S(476, 738),    S(486, 717),    //
    S(516, 767),    S(560, 792),    S(495, 895),    S(445, 910),    S(420, 905),    S(472, 822),    S(508, 740),    S(528, 659),    //
    S(554, 688),    S(557, 777),    S(494, 849),    S(487, 876),    S(478, 876),    S(490, 781),    S(531, 718),    S(519, 677),    //
    S(569, 655),    S(556, 727),    S(528, 782),    S(498, 825),    S(496, 828),    S(499, 773),    S(518, 691),    S(544, 615),    //
    S(549, 598),    S(585, 645),    S(573, 717),    S(544, 712),    S(541, 700),    S(536, 724),    S(551, 642),    S(528, 617),    //
    S(530, 486),    S(567, 500),    S(572, 566),    S(584, 604),    S(571, 632),    S(564, 596),    S(528, 634),    S(538, 613),    //
    S(513, 499),    S(545, 348),    S(567, 371),    S(582, 479),    S(577, 556),    S(576, 503),    S(564, 527),    S(533, 556),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-60, -326),   S(287, 173),    S(156, 135),    S(29, -32),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(231, -69),    S(348, 159),    S(200, 153),    S(119, 2),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(224, 27),     S(381, 104),    S(266, 107),    S(160, 6),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(80, -5),      S(333, 41),     S(181, 63),     S(118, 26),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-23, -28),    S(233, 6),      S(159, 16),     S(43, 23),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-26, -16),    S(210, -3),     S(112, 8),      S(62, -10),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-6, -18),     S(80, 36),      S(-4, 39),      S(-61, 14),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-118, -70),   S(-19, -23),    S(-120, -16),   S(-122, -88),   S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

inline const std::array<std::array<PParam, 7>, 4> KING_SHELTER = {{
  {{ S(15, -18), S(-3, 18), S(-7, 20), S(1, 5), S(1, 2), S(-6, 19), S(-1, 19), }},
  {{ S(5, -5), S(-22, 28), S(-19, 21), S(-9, 3), S(-9, 4), S(-18, 26), S(-21, 36), }},
  {{ S(-14, -5), S(-25, 21), S(-24, 1), S(-20, -12), S(-19, -11), S(-23, 1), S(-24, 17), }},
  {{ S(1, -15), S(-17, -15), S(-8, -19), S(-3, -23), S(-4, -21), S(-9, -17), S(-13, -17), }},
}};
inline const std::array<PParam, 7> BLOCKED_SHELTER_STORM = {
    S(0, 0), S(0, 0), S(3, 23), S(-19, 34), S(-16, 23), S(-8, 11), S(-8, -5),
};
inline const std::array<std::array<PParam, 7>, 4> SHELTER_STORM = {{
  {{ S(-5, 5), S(-15, 14), S(-14, 12), S(-13, 13), S(-13, 7), S(-12, 3), S(-15, 12), }},
  {{ S(-4, 4), S(-12, 10), S(-18, 15), S(-15, 12), S(-15, 9), S(-17, 12), S(-11, 9), }},
  {{ S(-10, 8), S(-15, 6), S(-16, 13), S(-12, 12), S(-11, 11), S(-15, 11), S(-14, 6), }},
  {{ S(-11, 10), S(-16, 12), S(-16, 11), S(-12, 15), S(-10, 15), S(-16, 11), S(-17, 11), }},
}};
inline TunableSigmoid<32> KING_SAFETY_ACTIVATION(
        1192, 549, -27, 56
);

inline VParam WINNABLE_PAWNS = V(-15);
inline VParam WINNABLE_SYM = V(138);
inline VParam WINNABLE_ASYM = V(125);
inline VParam WINNABLE_PAWN_ENDGAME = V(225);
inline VParam WINNABLE_BIAS = V(-657);

// Epoch duration: 7.72358s
// clang-format on
}  // namespace Clockwork
