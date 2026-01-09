#pragma once

#include "eval_types.hpp"

namespace Clockwork {

// clang-format off
inline const PParam PAWN_MAT   = S(170, 156);
inline const PParam KNIGHT_MAT = S(512, 314);
inline const PParam BISHOP_MAT = S(453, 269);
inline const PParam ROOK_MAT   = S(544, 412);
inline const PParam QUEEN_MAT  = S(1080, 561);
inline const PParam TEMPO_VAL  = S(63, 14);

inline const PParam BISHOP_PAIR_VAL   = S(77, 163);
inline const PParam ROOK_OPEN_VAL     = S(114, -26);
inline const PParam ROOK_SEMIOPEN_VAL = S(44, 17);

inline const PParam DOUBLED_PAWN_VAL = S(-32, -75);

inline const PParam POTENTIAL_CHECKER_VAL = S(-69, -10);
inline const PParam OUTPOST_KNIGHT_VAL    = S(51, 40);
inline const PParam OUTPOST_BISHOP_VAL    = S(61, 26);

inline const PParam PAWN_PUSH_THREAT_KNIGHT = S(49, 4);
inline const PParam PAWN_PUSH_THREAT_BISHOP = S(58, -25);
inline const PParam PAWN_PUSH_THREAT_ROOK   = S(38, 34);
inline const PParam PAWN_PUSH_THREAT_QUEEN  = S(70, -53);

inline const std::array<PParam, 6> PAWN_PHALANX = {
    S(22, 12), S(57, 33), S(73, 70), S(181, 153), S(481, 224), S(672, 676),
};
inline const std::array<PParam, 5> DEFENDED_PAWN = {
    S(70, 38), S(60, 33), S(73, 65), S(175, 117), S(593, -10),
};
inline const std::array<PParam, 6> PASSED_PAWN = {
    S(-81, -96), S(-81, -74), S(-54, 7), S(27, 76), S(102, 196), S(302, 277),
};
inline const std::array<PParam, 6> DEFENDED_PASSED_PUSH = {
    S(43, -38), S(38, -6), S(29, 20), S(26, 66), S(97, 129), S(211, 209),
};
inline const std::array<PParam, 6> BLOCKED_PASSED_PAWN = {
    S(10, -33), S(-1, 11), S(-4, -19), S(-7, -39), S(-14, -89), S(-268, -106),
};

inline const std::array<PParam, 8> FRIENDLY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-3, 114), S(-16, 90), S(-9, 33), S(-1, 6), S(11, 6), S(57, 2), S(19, -6),
};
inline const std::array<PParam, 8> ENEMY_KING_PASSED_PAWN_DISTANCE = {
    S(0, 0), S(-299, -9), S(-34, 23), S(-13, 50), S(34, 61), S(40, 80), S(48, 87), S(19, 84),
};

inline const std::array<PParam, 9> KNIGHT_MOBILITY = {
    S(12, -59), S(111, 94), S(168, 165), S(205, 193), S(252, 203), S(281, 236), S(317, 228), S(355, 230), S(400, 169),
};
inline const std::array<PParam, 14> BISHOP_MOBILITY = {
    S(-17, -121), S(53, 38), S(116, 80), S(144, 128), S(173, 157), S(191, 177), S(200, 193), S(218, 198), S(230, 209), S(250, 198), S(273, 187), S(332, 144), S(337, 142), S(408, 97),
};
inline const std::array<PParam, 15> ROOK_MOBILITY = {
    S(278, 99), S(201, 260), S(230, 280), S(246, 283), S(260, 293), S(265, 305), S(273, 313), S(284, 312), S(291, 317), S(302, 319), S(315, 318), S(324, 317), S(331, 316), S(350, 296), S(458, 201),
};
inline const std::array<PParam, 28> QUEEN_MOBILITY = {
    S(460, 121), S(525, 234), S(573, 270), S(601, 365), S(613, 424), S(630, 455), S(633, 497), S(642, 501), S(645, 525), S(651, 535), S(654, 548), S(660, 552), S(671, 540), S(676, 542), S(680, 539), S(684, 534), S(689, 527), S(688, 527), S(700, 506), S(719, 488), S(735, 467), S(750, 434), S(779, 414), S(899, 302), S(950, 248), S(944, 242), S(862, 289), S(961, 217),
};
inline const std::array<PParam, 9> KING_MOBILITY = {
    S(659, -333), S(244, -188), S(121, -85), S(81, -36), S(30, -21), S(-26, 4), S(-74, 40), S(-126, 67), S(-165, 55),
};

inline const std::array<PParam, 3> KNIGHT_KING_RING = {
    S(228, 227), S(347, 180), S(450, 118),
};
inline const std::array<PParam, 3> BISHOP_KING_RING = {
    S(475, 373), S(292, 235), S(166, 68),
};
inline const std::array<PParam, 5> ROOK_KING_RING = {
    S(317, 319), S(444, 311), S(485, 319), S(587, 370), S(744, 329),
};
inline const std::array<PParam, 6> QUEEN_KING_RING = {
    S(870, 745), S(585, 608), S(329, 459), S(138, 254), S(78, -11), S(12, -295),
};

inline const std::array<PParam, 5> PT_INNER_RING_ATTACKS = {
    S(-119, 61), S(27, -18), S(-223, -129), S(56, 38), S(-241, -153),
};
inline const std::array<PParam, 5> PT_OUTER_RING_ATTACKS = {
    S(-28, 22), S(-21, 20), S(-20, 15), S(-15, 9), S(-21, -12),
};

inline const PParam PAWN_THREAT_KNIGHT = S(245, 60);
inline const PParam PAWN_THREAT_BISHOP = S(218, 116);
inline const PParam PAWN_THREAT_ROOK   = S(203, 100);
inline const PParam PAWN_THREAT_QUEEN  = S(187, -45);

inline const PParam KNIGHT_THREAT_BISHOP = S(118, 73);
inline const PParam KNIGHT_THREAT_ROOK   = S(256, 15);
inline const PParam KNIGHT_THREAT_QUEEN  = S(155, -18);

inline const PParam BISHOP_THREAT_KNIGHT = S(115, 38);
inline const PParam BISHOP_THREAT_ROOK   = S(238, 72);
inline const PParam BISHOP_THREAT_QUEEN  = S(199, 52);

inline const std::array<PParam, 9> BISHOP_PAWNS = {
    S(3, -20), S(-3, -8), S(-4, -18), S(-9, -25), S(-15, -32), S(-21, -35), S(-24, -46), S(-29, -47), S(-37, -52),
};

inline const std::array<PParam, 48> PAWN_PSQT = {
    S(295, 277),    S(197, 347),    S(309, 312),    S(292, 250),    S(344, 179),    S(249, 255),    S(204, 279),    S(310, 229),    //
    S(159, 173),    S(210, 218),    S(200, 165),    S(164, 130),    S(147, 100),    S(117, 140),    S(96, 179),     S(61, 185),     //
    S(101, 138),    S(100, 157),    S(113, 116),    S(99, 108),     S(84, 96),      S(54, 110),     S(25, 143),     S(10, 161),     //
    S(88, 93),      S(101, 123),    S(106, 96),     S(88, 99),      S(59, 93),      S(41, 100),     S(-8, 142),     S(-22, 128),    //
    S(86, 68),      S(146, 73),     S(92, 119),     S(76, 119),     S(44, 113),     S(13, 112),     S(-11, 127),    S(-27, 113),    //
    S(85, 75),      S(210, 84),     S(151, 129),    S(105, 146),    S(74, 131),     S(50, 117),     S(30, 136),     S(-8, 129),     //
};
inline const std::array<PParam, 64> KNIGHT_PSQT = {
    S(-283, -47),   S(-219, 186),   S(-456, 438),   S(94, 149),     S(22, 155),     S(-109, 133),   S(-371, 196),   S(-338, 31),    //
    S(112, 122),    S(163, 122),    S(244, 82),     S(219, 118),    S(216, 130),    S(149, 123),    S(132, 131),    S(105, 105),    //
    S(181, 92),     S(194, 139),    S(251, 152),    S(212, 171),    S(186, 176),    S(129, 186),    S(140, 125),    S(110, 120),    //
    S(240, 133),    S(224, 151),    S(227, 177),    S(202, 210),    S(219, 199),    S(192, 184),    S(194, 139),    S(191, 114),    //
    S(232, 118),    S(267, 108),    S(244, 152),    S(229, 175),    S(216, 173),    S(218, 162),    S(219, 111),    S(181, 112),    //
    S(178, 92),     S(207, 89),     S(190, 126),    S(211, 150),    S(212, 147),    S(162, 134),    S(162, 99),     S(128, 82),     //
    S(188, 86),     S(203, 72),     S(185, 87),     S(183, 108),    S(170, 114),    S(149, 69),     S(155, 82),     S(115, 17),     //
    S(132, 64),     S(166, 122),    S(191, 75),     S(208, 63),     S(193, 82),     S(143, 63),     S(133, 99),     S(83, 36),      //
};
inline const std::array<PParam, 64> BISHOP_PSQT = {
    S(-33, 185),    S(-68, 169),    S(-358, 242),   S(-222, 209),   S(-189, 219),   S(-290, 253),   S(-104, 220),   S(-29, 195),    //
    S(77, 115),     S(25, 175),     S(59, 133),     S(-5, 181),     S(-20, 187),    S(39, 162),     S(68, 144),     S(51, 135),     //
    S(145, 127),    S(171, 125),    S(167, 157),    S(129, 150),    S(100, 147),    S(98, 156),     S(144, 133),    S(118, 113),    //
    S(124, 108),    S(143, 126),    S(163, 139),    S(143, 177),    S(172, 163),    S(112, 148),    S(119, 122),    S(90, 112),     //
    S(140, 82),     S(150, 110),    S(160, 129),    S(167, 151),    S(156, 159),    S(121, 149),    S(85, 130),     S(91, 97),      //
    S(158, 87),     S(193, 93),     S(211, 108),    S(160, 145),    S(151, 131),    S(157, 127),    S(166, 103),    S(105, 109),    //
    S(142, 39),     S(226, 57),     S(189, 70),     S(156, 97),     S(138, 107),    S(146, 80),     S(147, 86),     S(136, 46),     //
    S(148, 49),     S(134, 98),     S(139, 121),    S(158, 77),     S(166, 65),     S(158, 124),    S(152, 80),     S(131, 72),     //
};
inline const std::array<PParam, 64> ROOK_PSQT = {
    S(397, 245),    S(433, 254),    S(394, 277),    S(404, 260),    S(400, 259),    S(348, 270),    S(363, 272),    S(360, 275),    //
    S(291, 301),    S(354, 291),    S(445, 265),    S(380, 288),    S(382, 289),    S(343, 294),    S(261, 323),    S(260, 323),    //
    S(278, 287),    S(405, 254),    S(437, 246),    S(420, 244),    S(381, 254),    S(333, 278),    S(333, 279),    S(254, 312),    //
    S(256, 274),    S(332, 274),    S(372, 258),    S(352, 255),    S(347, 264),    S(305, 283),    S(285, 284),    S(226, 297),    //
    S(214, 229),    S(285, 239),    S(265, 257),    S(262, 245),    S(253, 254),    S(238, 281),    S(208, 272),    S(183, 269),    //
    S(182, 203),    S(246, 200),    S(249, 223),    S(236, 215),    S(240, 209),    S(212, 246),    S(203, 224),    S(162, 238),    //
    S(94, 219),     S(211, 161),    S(232, 183),    S(241, 186),    S(233, 187),    S(219, 201),    S(197, 184),    S(170, 200),    //
    S(144, 207),    S(161, 228),    S(234, 188),    S(261, 172),    S(241, 190),    S(229, 205),    S(214, 197),    S(194, 221),    //
};
inline const std::array<PParam, 64> QUEEN_PSQT = {
    S(461, 412),    S(539, 348),    S(528, 381),    S(446, 485),    S(461, 447),    S(449, 442),    S(486, 373),    S(388, 448),    //
    S(483, 441),    S(426, 546),    S(430, 558),    S(287, 621),    S(293, 613),    S(363, 568),    S(392, 484),    S(412, 462),    //
    S(421, 498),    S(512, 485),    S(439, 558),    S(406, 577),    S(359, 580),    S(376, 533),    S(450, 435),    S(427, 407),    //
    S(497, 395),    S(492, 465),    S(452, 508),    S(428, 560),    S(417, 563),    S(419, 497),    S(474, 402),    S(471, 370),    //
    S(480, 399),    S(489, 418),    S(465, 468),    S(437, 512),    S(439, 508),    S(436, 469),    S(456, 398),    S(467, 350),    //
    S(467, 318),    S(501, 351),    S(503, 420),    S(467, 401),    S(463, 394),    S(471, 416),    S(476, 358),    S(462, 332),    //
    S(449, 200),    S(492, 153),    S(495, 243),    S(506, 291),    S(488, 318),    S(488, 287),    S(459, 337),    S(459, 323),    //
    S(407, 241),    S(457, 42),     S(457, 68),     S(486, 159),    S(493, 234),    S(494, 186),    S(484, 218),    S(436, 267),    //
};
inline const std::array<PParam, 64> KING_PSQT = {
    S(-606, -153),  S(-133, 153),   S(57, 71),      S(-135, 39),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(21, -42),     S(173, 58),     S(172, 68),     S(139, -12),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-43, 86),     S(209, 61),     S(227, 53),     S(122, 18),     S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-294, 101),   S(131, 35),     S(63, 45),      S(56, 33),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-299, 68),    S(73, 1),       S(93, 4),       S(9, 38),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-154, 35),    S(159, -34),    S(118, -7),     S(89, 11),      S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-5, -26),     S(179, -57),    S(133, -29),    S(61, 7),       S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
    S(-190, -17),   S(-18, -21),    S(-108, 10),    S(-111, -4),    S(0, 0),        S(0, 0),        S(0, 0),        S(0, 0),        //
};

// Epoch duration: 8.68854s
// clang-format on
}  // namespace Clockwork
