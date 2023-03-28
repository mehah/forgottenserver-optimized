-- phpMyAdmin SQL Dump
-- version 4.8.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: 28-Mar-2023 às 23:29
-- Versão do servidor: 10.1.37-MariaDB
-- versão do PHP: 7.3.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `forgottenserver-optimized`
--
CREATE DATABASE IF NOT EXISTS `forgottenserver-optimized` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `forgottenserver-optimized`;

-- --------------------------------------------------------

--
-- Estrutura da tabela `accounts`
--

CREATE TABLE `accounts` (
  `id` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `password` char(40) NOT NULL,
  `secret` char(16) DEFAULT NULL,
  `type` int(11) NOT NULL DEFAULT '1',
  `premdays` int(11) NOT NULL DEFAULT '0',
  `lastday` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `email` varchar(255) NOT NULL DEFAULT '',
  `creation` int(11) NOT NULL DEFAULT '0',
  `coins` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `accounts`
--

INSERT INTO `accounts` (`id`, `name`, `password`, `secret`, `type`, `premdays`, `lastday`, `email`, `creation`, `coins`) VALUES
(1, 'teste', '2e6f9b0d5885b6010f9167787445617f553a735f', NULL, 5, 0, 0, '', 0, 0);

-- --------------------------------------------------------

--
-- Estrutura da tabela `account_bans`
--

CREATE TABLE `account_bans` (
  `account_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expires_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `account_ban_history`
--

CREATE TABLE `account_ban_history` (
  `id` int(10) UNSIGNED NOT NULL,
  `account_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expired_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `account_viplist`
--

CREATE TABLE `account_viplist` (
  `account_id` int(11) NOT NULL COMMENT 'id of account whose viplist entry it is',
  `player_id` int(11) NOT NULL COMMENT 'id of target player of viplist entry',
  `description` varchar(128) NOT NULL DEFAULT '',
  `icon` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `notify` tinyint(4) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guilds`
--

CREATE TABLE `guilds` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `ownerid` int(11) NOT NULL,
  `creationdata` int(11) NOT NULL,
  `motd` varchar(255) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Acionadores `guilds`
--
DELIMITER $$
CREATE TRIGGER `oncreate_guilds` AFTER INSERT ON `guilds` FOR EACH ROW BEGIN
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('the Leader', 3, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Vice-Leader', 2, NEW.`id`);
    INSERT INTO `guild_ranks` (`name`, `level`, `guild_id`) VALUES ('a Member', 1, NEW.`id`);
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guildwar_kills`
--

CREATE TABLE `guildwar_kills` (
  `id` int(11) NOT NULL,
  `killer` varchar(50) NOT NULL,
  `target` varchar(50) NOT NULL,
  `killerguild` int(11) NOT NULL DEFAULT '0',
  `targetguild` int(11) NOT NULL DEFAULT '0',
  `warid` int(11) NOT NULL DEFAULT '0',
  `time` bigint(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guild_invites`
--

CREATE TABLE `guild_invites` (
  `player_id` int(11) NOT NULL DEFAULT '0',
  `guild_id` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guild_membership`
--

CREATE TABLE `guild_membership` (
  `player_id` int(11) NOT NULL,
  `guild_id` int(11) NOT NULL,
  `rank_id` int(11) NOT NULL,
  `nick` varchar(15) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guild_ranks`
--

CREATE TABLE `guild_ranks` (
  `id` int(11) NOT NULL,
  `guild_id` int(11) NOT NULL COMMENT 'guild',
  `name` varchar(255) NOT NULL COMMENT 'rank name',
  `level` int(11) NOT NULL COMMENT 'rank level - leader, vice, member, maybe something else'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `guild_wars`
--

CREATE TABLE `guild_wars` (
  `id` int(11) NOT NULL,
  `guild1` int(11) NOT NULL DEFAULT '0',
  `guild2` int(11) NOT NULL DEFAULT '0',
  `name1` varchar(255) NOT NULL,
  `name2` varchar(255) NOT NULL,
  `status` tinyint(4) NOT NULL DEFAULT '0',
  `started` bigint(20) NOT NULL DEFAULT '0',
  `ended` bigint(20) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `houses`
--

CREATE TABLE `houses` (
  `id` int(11) NOT NULL,
  `owner` int(11) NOT NULL,
  `paid` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `warnings` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL,
  `rent` int(11) NOT NULL DEFAULT '0',
  `town_id` int(11) NOT NULL DEFAULT '0',
  `bid` int(11) NOT NULL DEFAULT '0',
  `bid_end` int(11) NOT NULL DEFAULT '0',
  `last_bid` int(11) NOT NULL DEFAULT '0',
  `highest_bidder` int(11) NOT NULL DEFAULT '0',
  `size` int(11) NOT NULL DEFAULT '0',
  `beds` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `houses`
--

INSERT INTO `houses` (`id`, `owner`, `paid`, `warnings`, `name`, `rent`, `town_id`, `bid`, `bid_end`, `last_bid`, `highest_bidder`, `size`, `beds`) VALUES
(1, 0, 0, 0, 'Rhyves Flats 14', 0, 2, 0, 0, 0, 0, 17, 1),
(2, 0, 0, 0, 'Rhyves Flats 15', 0, 2, 0, 0, 0, 0, 19, 1),
(3, 0, 0, 0, 'Rhyves Flats 16', 0, 2, 0, 0, 0, 0, 13, 1),
(4, 0, 0, 0, 'Baraganor Boulevard 1', 0, 3, 0, 0, 0, 0, 29, 1),
(5, 0, 0, 0, 'Saund Top 1', 0, 5, 0, 0, 0, 0, 16, 1),
(6, 0, 0, 0, 'Kazgal Close 1', 0, 3, 0, 0, 0, 0, 32, 1),
(7, 0, 0, 0, 'Tower Flat', 0, 2, 0, 0, 0, 0, 13, 1),
(8, 0, 0, 0, 'Castle Street 1', 0, 2, 0, 0, 0, 0, 29, 1),
(9, 0, 0, 0, 'Seaview 2', 0, 2, 0, 0, 0, 0, 42, 2),
(10, 0, 0, 0, 'Seaview 1', 0, 2, 0, 0, 0, 0, 29, 1),
(11, 0, 0, 0, 'Hill Hut 1', 0, 2, 0, 0, 0, 0, 26, 1),
(12, 0, 0, 0, 'Hill Hut 2', 0, 2, 0, 0, 0, 0, 26, 1),
(13, 0, 0, 0, 'Farm Lane 1', 0, 2, 0, 0, 0, 0, 20, 1),
(14, 0, 0, 0, 'Farm Lane 2', 0, 2, 0, 0, 0, 0, 34, 1),
(15, 0, 0, 0, 'Church Avenue 1', 0, 2, 0, 0, 0, 0, 21, 1),
(16, 0, 0, 0, 'Church Avenue 2', 0, 2, 0, 0, 0, 0, 21, 1),
(17, 0, 0, 0, 'Church Avenue 3', 0, 2, 0, 0, 0, 0, 37, 1),
(18, 0, 0, 0, 'Church Avenue 4', 0, 2, 0, 0, 0, 0, 29, 1),
(19, 0, 0, 0, 'Church Avenue 5', 0, 2, 0, 0, 0, 0, 19, 1),
(20, 0, 0, 0, 'Church Avenue 7', 0, 2, 0, 0, 0, 0, 23, 1),
(23, 0, 0, 0, 'City Walls 1', 0, 2, 0, 0, 0, 0, 29, 1),
(24, 0, 0, 0, 'City Walls 2', 0, 2, 0, 0, 0, 0, 29, 1),
(25, 0, 0, 0, 'City Walls 3', 0, 2, 0, 0, 0, 0, 39, 1),
(26, 0, 0, 0, 'City Walls 4', 0, 2, 0, 0, 0, 0, 39, 1),
(27, 0, 0, 0, 'Hilltop 1', 0, 2, 0, 0, 0, 0, 31, 1),
(28, 0, 0, 0, 'Hilltop 2', 0, 2, 0, 0, 0, 0, 36, 1),
(29, 0, 0, 0, 'Hilltop 3', 0, 2, 0, 0, 0, 0, 45, 1),
(30, 0, 0, 0, 'Hilltop Hall', 0, 2, 0, 0, 0, 0, 191, 2),
(31, 0, 0, 0, 'Rhyves Flats 1', 0, 2, 0, 0, 0, 0, 13, 1),
(32, 0, 0, 0, 'Rhyves Flats 2', 0, 2, 0, 0, 0, 0, 13, 1),
(33, 0, 0, 0, 'Rhyves Flats 3', 0, 2, 0, 0, 0, 0, 13, 1),
(34, 0, 0, 0, 'Rhyves Flats 4', 0, 2, 0, 0, 0, 0, 13, 1),
(35, 0, 0, 0, 'Rhyves Flats 5', 0, 2, 0, 0, 0, 0, 13, 1),
(36, 0, 0, 0, 'Rhyves Flats 6', 0, 2, 0, 0, 0, 0, 13, 1),
(37, 0, 0, 0, 'Rhyves Flats 7', 0, 2, 0, 0, 0, 0, 13, 1),
(38, 0, 0, 0, 'Rhyves Flats 8', 0, 2, 0, 0, 0, 0, 13, 1),
(39, 0, 0, 0, 'Rhyves Flats 9', 0, 2, 0, 0, 0, 0, 25, 1),
(40, 0, 0, 0, 'Rhyves Flats 10', 0, 2, 0, 0, 0, 0, 15, 1),
(41, 0, 0, 0, 'Rhyves Flats 11', 0, 2, 0, 0, 0, 0, 13, 1),
(42, 0, 0, 0, 'Rhyves Flats 13', 0, 2, 0, 0, 0, 0, 17, 1),
(43, 0, 0, 0, 'Rhyves Flats 12', 0, 2, 0, 0, 0, 0, 14, 1),
(44, 0, 0, 0, 'Baraganor Boulevard 2', 0, 3, 0, 0, 0, 0, 33, 1),
(45, 0, 0, 0, 'Baraganor Boulevard 3', 0, 3, 0, 0, 0, 0, 33, 1),
(46, 0, 0, 0, 'Baraganor Boulevard 4', 0, 3, 0, 0, 0, 0, 29, 1),
(47, 0, 0, 0, 'Baraganor Boulevard 5', 0, 3, 0, 0, 0, 0, 29, 1),
(48, 0, 0, 0, 'Baraganor Boulevard 6', 0, 3, 0, 0, 0, 0, 29, 1),
(49, 0, 0, 0, 'Baraganor Boulevard 7', 0, 3, 0, 0, 0, 0, 34, 1),
(50, 0, 0, 0, 'Baraganor Boulevard 8', 0, 3, 0, 0, 0, 0, 34, 1),
(51, 0, 0, 0, 'Kazgal Close 2', 0, 3, 0, 0, 0, 0, 34, 1),
(52, 0, 0, 0, 'Kazgal Close 3', 0, 3, 0, 0, 0, 0, 33, 1),
(53, 0, 0, 0, 'Kazgal Close 4', 0, 3, 0, 0, 0, 0, 34, 1),
(54, 0, 0, 0, 'Kazgal Close 5', 0, 3, 0, 0, 0, 0, 34, 1),
(55, 0, 0, 0, 'Kazgal Close6', 0, 3, 0, 0, 0, 0, 34, 1),
(56, 0, 0, 0, 'Kazgal Close 7', 0, 3, 0, 0, 0, 0, 34, 1),
(57, 0, 0, 0, 'Kazgal Close 8', 0, 3, 0, 0, 0, 0, 34, 1),
(58, 0, 0, 0, 'Hammersmith Hall', 0, 3, 0, 0, 0, 0, 203, 4),
(59, 0, 0, 0, 'Varnack\'s Cavern', 0, 3, 0, 0, 0, 0, 224, 4),
(60, 0, 0, 0, 'Saund Street 1', 0, 5, 0, 0, 0, 0, 13, 1),
(61, 0, 0, 0, 'Saund Street 2', 0, 5, 0, 0, 0, 0, 17, 1),
(62, 0, 0, 0, 'Saund Street 3', 0, 5, 0, 0, 0, 0, 13, 1),
(63, 0, 0, 0, 'Saund Street 4', 0, 0, 0, 0, 0, 0, 17, 1),
(64, 0, 0, 0, 'Saund Street 5', 0, 5, 0, 0, 0, 0, 13, 2),
(65, 0, 0, 0, 'Saund Top 2', 0, 5, 0, 0, 0, 0, 17, 1),
(66, 0, 0, 0, 'Saund Top 3', 0, 5, 0, 0, 0, 0, 16, 2),
(67, 0, 0, 0, 'Dock Street 1', 0, 4, 0, 0, 0, 0, 21, 1),
(68, 0, 0, 0, 'Dock Street 2', 0, 4, 0, 0, 0, 0, 23, 1),
(69, 0, 0, 0, 'Dock Street 3', 0, 4, 0, 0, 0, 0, 45, 1),
(70, 0, 0, 0, 'Nomad Hill 1', 0, 4, 0, 0, 0, 0, 18, 1),
(71, 0, 0, 0, 'Nomad Hill 2', 0, 4, 0, 0, 0, 0, 20, 1),
(72, 0, 0, 0, 'Nomad Hill 3', 0, 4, 0, 0, 0, 0, 28, 2),
(73, 0, 0, 0, 'Great Hall 1', 0, 4, 0, 0, 0, 0, 26, 1),
(74, 0, 0, 0, 'Great Hall 2', 0, 4, 0, 0, 0, 0, 30, 2),
(75, 0, 0, 0, 'Tarat Road 1', 0, 4, 0, 0, 0, 0, 26, 1),
(76, 0, 0, 0, 'Temple Terrace 1', 0, 4, 0, 0, 0, 0, 36, 1),
(77, 0, 0, 0, 'Temple Terrace 2', 0, 4, 0, 0, 0, 0, 33, 1),
(78, 0, 0, 0, 'Temple Terrace 3', 0, 4, 0, 0, 0, 0, 33, 1),
(79, 0, 0, 0, 'Tarat Road 2', 0, 4, 0, 0, 0, 0, 26, 1),
(80, 0, 0, 0, 'Tarat Road 3', 0, 4, 0, 0, 0, 0, 35, 1),
(81, 0, 0, 0, 'Tarat Road 4', 0, 4, 0, 0, 0, 0, 23, 1),
(82, 0, 0, 0, 'Tarat Road 6', 0, 4, 0, 0, 0, 0, 50, 2),
(83, 0, 0, 0, 'Tarat Road 7', 0, 4, 0, 0, 0, 0, 16, 1),
(84, 0, 0, 0, 'Tarat Road 8', 0, 4, 0, 0, 0, 0, 29, 1),
(85, 0, 0, 0, 'Tarat Road 9', 0, 4, 0, 0, 0, 0, 25, 2),
(86, 0, 0, 0, 'Smith Lane 1', 0, 4, 0, 0, 0, 0, 26, 1),
(87, 0, 0, 0, 'Smith Lane 2', 0, 4, 0, 0, 0, 0, 31, 1),
(88, 0, 0, 0, 'Smith Lane 3', 0, 0, 0, 0, 0, 0, 37, 2),
(89, 0, 0, 0, 'Smith Lane 4', 0, 4, 0, 0, 0, 0, 29, 1),
(90, 0, 0, 0, 'Smith Lane 5', 0, 4, 0, 0, 0, 0, 21, 1),
(91, 0, 0, 0, 'Smith Lane 6', 0, 0, 0, 0, 0, 0, 30, 2),
(92, 0, 0, 0, 'Smith Lane 7', 0, 4, 0, 0, 0, 0, 30, 1),
(93, 0, 0, 0, 'Smith Lane Shop', 0, 4, 0, 0, 0, 0, 37, 1),
(94, 0, 0, 0, 'Snowcapped Street 1', 0, 4, 0, 0, 0, 0, 26, 1),
(95, 0, 0, 0, 'Snowcapped Street 2', 0, 4, 0, 0, 0, 0, 21, 1),
(96, 0, 0, 0, 'Snowcapped Street 3', 0, 4, 0, 0, 0, 0, 21, 1),
(97, 0, 0, 0, 'Wall Flat 1', 0, 4, 0, 0, 0, 0, 17, 1),
(98, 0, 0, 0, 'Wall Flat 2', 0, 4, 0, 0, 0, 0, 17, 1),
(99, 0, 0, 0, 'Wall Flat 3', 0, 4, 0, 0, 0, 0, 32, 1),
(100, 0, 0, 0, 'Wall Flat 4', 0, 4, 0, 0, 0, 0, 13, 1),
(101, 0, 0, 0, 'Farm Lane 3', 0, 2, 0, 0, 0, 0, 29, 2),
(102, 0, 0, 0, 'Farm Lane 4', 0, 2, 0, 0, 0, 0, 35, 1),
(103, 0, 0, 0, 'The Square 1', 0, 2, 0, 0, 0, 0, 25, 1),
(104, 0, 0, 0, 'The Square 2 (Shop)', 0, 2, 0, 0, 0, 0, 37, 2),
(105, 0, 0, 0, 'Central Hall', 0, 2, 0, 0, 0, 0, 209, 8),
(106, 0, 0, 0, 'The Square 3', 0, 2, 0, 0, 0, 0, 21, 1),
(107, 0, 0, 0, 'The Square 4 (Shop)', 0, 2, 0, 0, 0, 0, 42, 1),
(108, 0, 0, 0, 'Church Avenue 6', 0, 2, 0, 0, 0, 0, 24, 1),
(109, 0, 0, 0, 'Castle Street 2', 0, 2, 0, 0, 0, 0, 26, 1),
(110, 0, 0, 0, 'Castle Street 3', 0, 2, 0, 0, 0, 0, 28, 1),
(111, 0, 0, 0, 'Armory Flat 1', 0, 2, 0, 0, 0, 0, 26, 2),
(112, 0, 0, 0, 'Armory Flat 2', 0, 2, 0, 0, 0, 0, 24, 1);

-- --------------------------------------------------------

--
-- Estrutura da tabela `house_lists`
--

CREATE TABLE `house_lists` (
  `house_id` int(11) NOT NULL,
  `listid` int(11) NOT NULL,
  `list` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `ip_bans`
--

CREATE TABLE `ip_bans` (
  `ip` int(10) UNSIGNED NOT NULL,
  `reason` varchar(255) NOT NULL,
  `banned_at` bigint(20) NOT NULL,
  `expires_at` bigint(20) NOT NULL,
  `banned_by` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `market_history`
--

CREATE TABLE `market_history` (
  `id` int(10) UNSIGNED NOT NULL,
  `player_id` int(11) NOT NULL,
  `sale` tinyint(4) NOT NULL DEFAULT '0',
  `itemtype` int(10) UNSIGNED NOT NULL,
  `amount` smallint(5) UNSIGNED NOT NULL,
  `price` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `expires_at` bigint(20) UNSIGNED NOT NULL,
  `inserted` bigint(20) UNSIGNED NOT NULL,
  `state` tinyint(3) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `market_offers`
--

CREATE TABLE `market_offers` (
  `id` int(10) UNSIGNED NOT NULL,
  `player_id` int(11) NOT NULL,
  `sale` tinyint(4) NOT NULL DEFAULT '0',
  `itemtype` int(10) UNSIGNED NOT NULL,
  `amount` smallint(5) UNSIGNED NOT NULL,
  `created` bigint(20) UNSIGNED NOT NULL,
  `anonymous` tinyint(4) NOT NULL DEFAULT '0',
  `price` int(10) UNSIGNED NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `players`
--

CREATE TABLE `players` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `group_id` smallint(5) UNSIGNED NOT NULL DEFAULT '1',
  `account_id` int(11) NOT NULL DEFAULT '0',
  `level` int(10) UNSIGNED NOT NULL DEFAULT '1',
  `vocation` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `health` int(11) NOT NULL DEFAULT '150',
  `healthmax` int(11) NOT NULL DEFAULT '150',
  `experience` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `lookbody` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lookfeet` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lookhead` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `looklegs` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `looktype` smallint(5) UNSIGNED NOT NULL DEFAULT '136',
  `lookaddons` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `maglevel` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `mana` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `manamax` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `manaspent` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `soul` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `town_id` int(10) UNSIGNED NOT NULL DEFAULT '1',
  `posx` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `posy` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `posz` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `conditions` blob NOT NULL,
  `cap` int(10) UNSIGNED NOT NULL DEFAULT '400',
  `sex` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lastlogin` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `lastip` int(10) UNSIGNED NOT NULL DEFAULT '0',
  `save` tinyint(4) NOT NULL DEFAULT '1',
  `skull` tinyint(4) NOT NULL DEFAULT '0',
  `skulltime` bigint(20) NOT NULL DEFAULT '0',
  `lastlogout` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `blessings` tinyint(4) NOT NULL DEFAULT '0',
  `onlinetime` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `deletion` bigint(20) NOT NULL DEFAULT '0',
  `balance` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `offlinetraining_time` smallint(5) UNSIGNED NOT NULL DEFAULT '43200',
  `offlinetraining_skill` int(11) NOT NULL DEFAULT '-1',
  `stamina` smallint(5) UNSIGNED NOT NULL DEFAULT '2520',
  `skill_fist` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_fist_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_club` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_club_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_sword` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_sword_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_axe` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_axe_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_dist` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_dist_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_shielding` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_shielding_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `skill_fishing` smallint(5) UNSIGNED NOT NULL DEFAULT '10',
  `skill_fishing_tries` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `direction` tinyint(3) UNSIGNED NOT NULL DEFAULT '2',
  `spells` blob,
  `storages` mediumblob,
  `items` longblob,
  `depotitems` longblob,
  `inboxitems` longblob,
  `depotlockeritems` longblob,
  `supplystash` longblob,
  `lookmountbody` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lookmountfeet` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lookmounthead` tinyint(3) UNSIGNED NOT NULL DEFAULT '0',
  `lookmountlegs` tinyint(3) UNSIGNED NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `players`
--

INSERT INTO `players` (`id`, `name`, `group_id`, `account_id`, `level`, `vocation`, `health`, `healthmax`, `experience`, `lookbody`, `lookfeet`, `lookhead`, `looklegs`, `looktype`, `lookaddons`, `maglevel`, `mana`, `manamax`, `manaspent`, `soul`, `town_id`, `posx`, `posy`, `posz`, `conditions`, `cap`, `sex`, `lastlogin`, `lastip`, `save`, `skull`, `skulltime`, `lastlogout`, `blessings`, `onlinetime`, `deletion`, `balance`, `offlinetraining_time`, `offlinetraining_skill`, `stamina`, `skill_fist`, `skill_fist_tries`, `skill_club`, `skill_club_tries`, `skill_sword`, `skill_sword_tries`, `skill_axe`, `skill_axe_tries`, `skill_dist`, `skill_dist_tries`, `skill_shielding`, `skill_shielding_tries`, `skill_fishing`, `skill_fishing_tries`, `direction`, `spells`, `storages`, `items`, `depotitems`, `inboxitems`, `depotlockeritems`, `supplystash`, `lookmountbody`, `lookmountfeet`, `lookmounthead`, `lookmountlegs`) VALUES
(1, 'GOD', 3, 1, 99999, 4, 150, 150, 16665166719999400, 0, 0, 0, 0, 136, 0, 0, 0, 0, 0, 0, 1, 242, 444, 12, '', 400, 0, 1680038688, 16777343, 1, 0, 0, 1680038699, 0, 4832, 0, 0, 43200, -1, 2520, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 2, NULL, 0x0000000000000000, 0x03000000c3071701000000720a0f010000040000005b0a00060000004e09000a0000000208000b000000c465170000000000, NULL, NULL, NULL, NULL, 0, 0, 0, 0),
(2, 'Player Low Level', 1, 1, 20, 4, 150, 150, 98800, 0, 0, 0, 0, 136, 0, 0, 0, 0, 0, 0, 1, 100, 116, 7, '', 400, 0, 1679530455, 16777343, 1, 0, 0, 1679530461, 0, 8709, 0, 0, 43200, -1, 2520, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 3, NULL, 0x0000000000000000, 0x0b000000c465170000000000, NULL, NULL, NULL, NULL, 0, 0, 0, 0),
(3, 'Player Medium Level', 1, 1, 250, 4, 9999999, 9999999, 254237300, 0, 0, 0, 0, 136, 0, 0, 9999999, 9999999, 0, 0, 1, 95, 117, 7, '', 400, 0, 1680038700, 16777343, 1, 0, 0, 1680038710, 0, 8719, 0, 0, 43200, -1, 2520, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 10, 0, 2, NULL, 0x0000000000000000, 0x0b000000c465170000000000, NULL, NULL, NULL, NULL, 0, 0, 0, 0);

--
-- Acionadores `players`
--
DELIMITER $$
CREATE TRIGGER `ondelete_players` BEFORE DELETE ON `players` FOR EACH ROW BEGIN
    UPDATE `houses` SET `owner` = 0 WHERE `owner` = OLD.`id`;
END
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Estrutura da tabela `players_online`
--

CREATE TABLE `players_online` (
  `player_id` int(11) NOT NULL
) ENGINE=MEMORY DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `player_deaths`
--

CREATE TABLE `player_deaths` (
  `player_id` int(11) NOT NULL,
  `time` bigint(20) UNSIGNED NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '1',
  `killed_by` varchar(255) NOT NULL,
  `is_player` tinyint(4) NOT NULL DEFAULT '1',
  `mostdamage_by` varchar(100) NOT NULL,
  `mostdamage_is_player` tinyint(4) NOT NULL DEFAULT '0',
  `unjustified` tinyint(4) NOT NULL DEFAULT '0',
  `mostdamage_unjustified` tinyint(4) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `player_namelocks`
--

CREATE TABLE `player_namelocks` (
  `player_id` int(11) NOT NULL,
  `reason` varchar(255) NOT NULL,
  `namelocked_at` bigint(20) NOT NULL,
  `namelocked_by` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `server_config`
--

CREATE TABLE `server_config` (
  `config` varchar(50) NOT NULL,
  `value` varchar(256) NOT NULL DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `server_config`
--

INSERT INTO `server_config` (`config`, `value`) VALUES
('db_version', '32'),
('motd_hash', 'bd746a8e830237f7a1380be98802a0483741cff8'),
('motd_num', '1'),
('players_record', '2');

-- --------------------------------------------------------

--
-- Estrutura da tabela `store_history`
--

CREATE TABLE `store_history` (
  `id` int(10) UNSIGNED NOT NULL,
  `account_id` int(11) NOT NULL,
  `mode` smallint(6) NOT NULL DEFAULT '0',
  `description` varchar(3500) NOT NULL,
  `coin_amount` int(11) NOT NULL,
  `time` bigint(20) UNSIGNED NOT NULL,
  `coin_type` smallint(6) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Estrutura da tabela `tile_store`
--

CREATE TABLE `tile_store` (
  `house_id` int(11) NOT NULL,
  `data` longblob NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `tile_store`
--

INSERT INTO `tile_store` (`house_id`, `data`) VALUES
(1, 0xa50063010601000000de0600),
(1, 0xa50064010601000000df0600),
(1, 0xa70067010601000000bc0400),
(2, 0xa4006a010601000000bc0400),
(2, 0xa1006d010601000000dc0600),
(2, 0xa2006d010601000000dd0600),
(3, 0xa10063010601000000de0600),
(3, 0xa10064010601000000df0600),
(3, 0xa30067010601000000bc0400),
(4, 0xfb00e1010c01000000f31300),
(4, 0xfd00e1010b01000000f31300),
(4, 0xff00e1010b01000000da0600),
(4, 0xff00e2010b01000000db0600),
(5, 0xdf002f020501000000de0600),
(5, 0xdf0030020501000000df0600),
(5, 0xe40030020501000000b90400),
(6, 0xe400e1010b01000000da0600),
(6, 0xe800e1010c01000000f31300),
(7, 0x87005f010401000000da0600),
(7, 0x840060010401000000691800),
(7, 0x870060010401000000db0600),
(8, 0x70005c010701000000da0600),
(8, 0x70005d010701000000db0600),
(8, 0x74005d010701000000691800),
(9, 0x76005f010601000000e00600),
(9, 0x77005f010601000000e10600),
(9, 0x7200630106010000006c1800),
(9, 0x740061010601000000691800),
(9, 0x760062010601000000e00600),
(9, 0x770062010601000000e10600),
(10, 0x70005c010501000000e00600),
(10, 0x71005c010501000000e10600),
(10, 0x740061010501000000691800),
(11, 0x82006b010601000000691800),
(11, 0x870069010601000000da0600),
(11, 0x87006a010601000000db0600),
(12, 0x8600680105010000006c1800),
(12, 0x83006d010501000000e00600),
(12, 0x84006d010501000000e10600),
(13, 0x6e0077010701000000dc0600),
(13, 0x6f0077010701000000dd0600),
(13, 0x710079010701000000bd0400),
(13, 0x74007b010701000000ba0400),
(14, 0x6e0088010601000000bd0400),
(14, 0x6d008c010601000000dc0600),
(14, 0x6e008c010601000000dd0600),
(14, 0x730087010701000000ba0400),
(15, 0x940077010701000000b90400),
(15, 0x980075010701000000de0600),
(15, 0x980076010701000000df0600),
(16, 0x94007e010701000000b90400),
(16, 0x98007e010701000000de0600),
(16, 0x98007f010701000000df0600),
(17, 0x950089010701000000de0600),
(17, 0x95008a010701000000df0600),
(17, 0x97008f010701000000bc0400),
(18, 0x970095010701000000bc0400),
(18, 0x940098010701000000dc0600),
(18, 0x950098010701000000dd0600),
(19, 0x9c008c010701000000de0600),
(19, 0x9c008d010701000000df0600),
(19, 0x9f008f010701000000bc0400),
(20, 0x9f0099010701000000de0600),
(20, 0x9f009a010701000000df0600),
(20, 0xa20095010701000000bc0400),
(23, 0x6c009d010701000000da0600),
(23, 0x6c009e010701000000db0600),
(23, 0x72009e010701000000691800),
(24, 0x6c009d010601000000da0600),
(24, 0x6c009e010601000000db0600),
(24, 0x72009e010601000000691800),
(25, 0xbd0065010701000000da0600),
(25, 0xbd0066010701000000db0600),
(25, 0xc100690107010000006c1800),
(26, 0xbd0065010601000000da0600),
(26, 0xbd0066010601000000db0600),
(26, 0xc100690106010000006c1800),
(27, 0x9f0096010601000000dc0600),
(27, 0xa00096010601000000dd0600),
(27, 0xa50099010601000000b90400),
(28, 0xb40088010601000000da0600),
(28, 0xb40089010601000000db0600),
(28, 0xb1008f0106010000006c1800),
(29, 0xb3009c010601000000691800),
(29, 0xb4009d010501000000e00600),
(29, 0xb5009d010501000000e10600),
(30, 0xbc0098010401000000691800),
(30, 0xbc009c010401000000691800),
(30, 0xbe009e0105010000006c1800),
(30, 0xbc00a1010501000000dc0600),
(30, 0xbd00a1010501000000dd0600),
(30, 0xbf00a1010501000000dc0600),
(30, 0xc000960105010000006c1800),
(30, 0xc100960105010000006c1800),
(30, 0xc3009b010401000000691800),
(30, 0xc000a1010501000000dd0600),
(31, 0xa1006d010701000000dc0600),
(31, 0xa2006d010701000000dd0600),
(31, 0xa5006c010701000000b90400),
(32, 0xa10067010701000000dc0600),
(32, 0xa20067010701000000dd0600),
(32, 0xa50068010701000000b90400),
(33, 0xa10063010701000000dc0600),
(33, 0xa20063010701000000dd0600),
(33, 0xa50065010701000000b90400),
(34, 0xa90067010701000000de0600),
(34, 0xa90068010701000000df0600),
(34, 0xab006a010701000000bc0400),
(35, 0xb3006b010701000000dc0600),
(35, 0xb4006b010701000000dd0600),
(35, 0xb0006c010701000000b90400),
(36, 0xb30067010701000000dc0600),
(36, 0xb40067010701000000dd0600),
(36, 0xb00068010701000000b90400),
(37, 0xb30063010701000000dc0600),
(37, 0xb40063010701000000dd0600),
(37, 0xb00064010701000000b90400),
(38, 0xa90063010701000000dc0600),
(38, 0xaa0063010701000000dd0600),
(38, 0xad0065010701000000b90400),
(39, 0xb40063010601000000de0600),
(39, 0xb00065010601000000b90400),
(39, 0xb40064010601000000df0600),
(39, 0xfe00b5010c010000001a0700),
(40, 0xb40067010601000000de0600),
(40, 0xb00068010601000000b90400),
(40, 0xb40068010601000000df0600),
(41, 0xb4006b010601000000de0600),
(41, 0xb0006c010601000000b90400),
(41, 0xb4006c010601000000df0600),
(42, 0xaa006a010601000000bc0400),
(42, 0xa8006d010601000000dc0600),
(42, 0xa9006d010601000000dd0600),
(43, 0xaa0063010601000000de0600),
(43, 0xaa0064010601000000df0600),
(43, 0xab0067010601000000bc0400),
(44, 0x0401da010b01000000ea1300),
(44, 0x0401d8010c01000000ea1300),
(44, 0x0401dc010b01000000e00600),
(44, 0x0501dc010b01000000e10600),
(45, 0x0a01da010b01000000ea1300),
(45, 0x0a01d8010c01000000ea1300),
(45, 0x0a01dc010b01000000e00600),
(45, 0x0b01dc010b01000000e10600),
(46, 0x0f01d5010b01000000f31300),
(46, 0x0d01d5010c01000000f31300),
(46, 0x1101d5010b01000000da0600),
(46, 0x1101d6010b01000000db0600),
(47, 0x0f01cf010b01000000f31300),
(47, 0x0d01cf010c01000000f31300),
(47, 0x1101cf010b01000000da0600),
(47, 0x1101d0010b01000000db0600),
(48, 0x0f01c9010b01000000f31300),
(48, 0x0d01c9010c01000000f31300),
(48, 0x1101c9010b01000000da0600),
(48, 0x1101ca010b01000000db0600),
(49, 0x0a01c1010b01000000e00600),
(49, 0x0a01c3010b01000000ea1300),
(49, 0x0b01c1010b01000000e10600),
(49, 0x0a01c5010c01000000ea1300),
(50, 0x0401c1010b01000000e00600),
(50, 0x0401c3010b01000000ea1300),
(50, 0x0501c1010b01000000e10600),
(50, 0x0401c5010c01000000ea1300),
(51, 0xe000da010b01000000ea1300),
(51, 0xe000d8010c01000000ea1300),
(51, 0xe000dc010b01000000e00600),
(51, 0xe100dc010b01000000e10600),
(52, 0xda00da010b01000000ea1300),
(52, 0xda00d8010c01000000ea1300),
(52, 0xda00dc010b01000000e00600),
(52, 0xdb00dc010b01000000e10600),
(53, 0xd200d5010b01000000da0600),
(53, 0xd200d6010b01000000db0600),
(53, 0xd600d5010c01000000f31300),
(54, 0xd200cf010b01000000da0600),
(54, 0xd400cf010b01000000f31300),
(54, 0xd600cf010c01000000f31300),
(54, 0xd200d0010b01000000db0600),
(55, 0xd200c9010b01000000da0600),
(55, 0xd200ca010b01000000db0600),
(55, 0xd400c9010b01000000f31300),
(55, 0xd600c9010c01000000f31300),
(56, 0xda00c1010b01000000e00600),
(56, 0xda00c3010b01000000ea1300),
(56, 0xdb00c1010b01000000e10600),
(56, 0xda00c5010c01000000ea1300),
(57, 0xe000c1010b01000000e00600),
(57, 0xe000c3010b01000000ea1300),
(57, 0xe100c1010b01000000e10600),
(57, 0xe000c5010c01000000ea1300),
(58, 0xde00b6010c01000000b90400),
(58, 0xdb00b8010b01000000bc0400),
(58, 0xd900bd010b01000000e00600),
(58, 0xda00bd010b01000000e10600),
(58, 0xdb00bd010b01000000e00600),
(58, 0xdc00bd010b01000000e10600),
(58, 0xde00bd010b01000000e00600),
(58, 0xdf00bd010b01000000e10600),
(58, 0xe500b6010c01000000b90400),
(58, 0xe500b7010c01000000b90400),
(58, 0xe000b8010b01000000bc0400),
(58, 0xe300b9010c01000000bc0400),
(58, 0xe000bd010b01000000e00600),
(58, 0xe100bd010b01000000e10600),
(59, 0xfe00bc010c01000000b90400),
(59, 0x0101b6010b01000000b90400),
(59, 0x0401b5010c01000000b90400),
(59, 0x0101b9010c01000000bc0400),
(59, 0x0401b8010b01000000bc0400),
(59, 0x0901b8010b01000000bc0400),
(59, 0x0201bd010b01000000e00600),
(59, 0x0301bd010b01000000e10600),
(59, 0x0401bd010b01000000e00600),
(59, 0x0501bd010b01000000e10600),
(59, 0x0701bd010b01000000e00600),
(59, 0x0401bc010c01000000b90400),
(59, 0x0801bd010b01000000e10600),
(59, 0x0a01bd010b01000000e00600),
(59, 0x0b01bd010b01000000e10600),
(60, 0xe50021020601000000dc0600),
(60, 0xe60021020601000000dd0600),
(60, 0xe70025020601000000bc0400),
(61, 0xe00021020601000000dc0600),
(61, 0xe10021020601000000dd0600),
(61, 0xe20025020601000000bc0400),
(62, 0xf2002a020601000000b90400),
(62, 0xf60029020601000000de0600),
(62, 0xf6002a020601000000df0600),
(63, 0xe9002f020601000000dc0600),
(63, 0xea002f020601000000dd0600),
(63, 0xed002e020601000000b90400),
(64, 0xf2002e020601000000b90400),
(64, 0xf5002d020601000000dc0600),
(64, 0xf6002d020602000000dd0600dd0600),
(65, 0xe8002e020501000000b90400),
(65, 0xec002c020501000000de0600),
(65, 0xec002d020501000000df0600),
(66, 0xdf002b020501000000dc0600),
(66, 0xdf002d020501000000dc0600),
(66, 0xe0002b020501000000dd0600),
(66, 0xe0002d020501000000dd0600),
(66, 0xe4002d020501000000b90400),
(67, 0xac01e9000601000000dc0600),
(67, 0xad01e9000601000000dd0600),
(67, 0xae01ed000601000000eb1a00),
(68, 0x9b01e6000701000000de0600),
(68, 0x9b01e7000701000000df0600),
(68, 0x9b01eb000701000000eb1a00),
(68, 0x9d01ed000701000000f41a00),
(69, 0x9c01d2000601000000dc0600),
(69, 0x9d01d2000601000000dd0600),
(69, 0x9b01d5000601000000f41a00),
(69, 0x9e01d4000701000000f41a00),
(70, 0x9401d7000601000000dc0600),
(70, 0x9501d7000601000000dd0600),
(70, 0x9501d90006010000008b1a00),
(70, 0x9601dd000601000000901b00),
(71, 0x8801e7000601000000dc0600),
(71, 0x8901e7000601000000dd0600),
(71, 0x8b01e50006010000008d1a00),
(71, 0x8f01e50006010000008e1b00),
(72, 0x8601d7000501000000dc0600),
(72, 0x8701d7000501000000dd0600),
(72, 0x8601da000501000000dc0600),
(72, 0x8701da000501000000dd0600),
(72, 0x8a01da0005010000008d1a00),
(72, 0x8d01dd000501000000901b00),
(73, 0xad01d2000601000000de0600),
(73, 0xad01d3000601000000df0600),
(73, 0xb101d3000601000000f41a00),
(73, 0xb401d1000601000000eb1a00),
(74, 0xb601cf000601000000f41a00),
(74, 0xb701d3000601000000de0600),
(74, 0xb901d1000601000000eb1a00),
(74, 0xba01d3000601000000de0600),
(74, 0xb701d4000601000000df0600),
(74, 0xba01d4000601000000df0600),
(75, 0xac01c1000701000000de0600),
(75, 0xac01c2000701000000df0600),
(75, 0xb101c3000701000000f41a00),
(76, 0xdf01be000701000000f41a00),
(76, 0xe401bb000701000000de0600),
(76, 0xe401bc000701000000df0600),
(77, 0xe301b7000701000000f41a00),
(77, 0xe601bb000701000000dc0600),
(77, 0xe701bb000701000000dd0600),
(78, 0xe701b1000601000000e00600),
(78, 0xe801b1000601000000e10600),
(78, 0xeb01b0000601000000851b00),
(79, 0xb901bb000701000000de0600),
(79, 0xb901bc000701000000df0600),
(79, 0xb801c0000701000000eb1a00),
(80, 0xbb01bb000701000000dc0600),
(80, 0xbc01bb000701000000dd0600),
(80, 0xbc01be000701000000eb1a00),
(80, 0xbe01c3000701000000eb1a00),
(81, 0xbf01bd000701000000dc0600),
(81, 0xc001bd000701000000dd0600),
(81, 0xc101bc000701000000f41a00),
(81, 0xc301bf000701000000eb1a00),
(82, 0xc401c9000701000000eb1a00),
(82, 0xc501ce000701000000eb1a00),
(82, 0xc701cf000701000000de0600),
(82, 0xc601d1000701000000dc0600),
(82, 0xc701d0000701000000df0600),
(82, 0xc701d1000701000000dd0600),
(83, 0xc701ba000701000000dc0600),
(83, 0xc801ba000701000000dd0600),
(83, 0xc701bf000701000000eb1a00),
(84, 0xce01c8000701000000f41a00),
(84, 0xd401c8000701000000de0600),
(84, 0xd401c9000701000000df0600),
(85, 0xbb01bb000601000000dc0600),
(85, 0xbc01bb000601000000dd0600),
(85, 0xbf01bb000601000000dc0600),
(85, 0xbe01bf000601000000eb1a00),
(85, 0xc001bb000601000000dd0600),
(86, 0xd501b4000701000000dc0600),
(86, 0xd601b4000701000000dd0600),
(86, 0xda01b7000701000000f41a00),
(87, 0xda01af000701000000f41a00),
(87, 0xd501b2000701000000dc0600),
(87, 0xd601b2000701000000dd0600),
(88, 0xd201a6000701000000dc0600),
(88, 0xd301a6000701000000dd0600),
(88, 0xd201ab000701000000dc0600),
(88, 0xd301ab000701000000dd0600),
(88, 0xd801a8000701000000f41a00),
(89, 0xd301a1000701000000de0600),
(89, 0xd301a2000701000000df0600),
(89, 0xda01a3000701000000f41a00),
(90, 0xd5019c000701000000de0600),
(90, 0xd5019d000701000000df0600),
(90, 0xda019e000701000000f41a00),
(91, 0xd801a4000601000000f41a00),
(91, 0xd401aa000601000000de0600),
(91, 0xd401ab000601000000df0600),
(91, 0xd601a8000601000000eb1a00),
(91, 0xd701aa000601000000de0600),
(91, 0xd701ab000601000000df0600),
(92, 0xd401a1000601000000de0600),
(92, 0xd401a2000601000000df0600),
(92, 0xd601a0000601000000eb1a00),
(92, 0xd901a0000601000000eb1a00),
(93, 0xe3019f000701000000620600),
(93, 0xe6019f000701000000f41a00),
(93, 0xe7019d000701000000dc0600),
(93, 0xe8019d000701000000dd0600),
(93, 0xe001a0000701000000f41a00),
(94, 0xab01c3000601000000f41a00),
(94, 0xb001c4000601000000de0600),
(94, 0xb001c5000601000000df0600),
(95, 0xab01ba000601000000de0600),
(95, 0xab01bb000601000000df0600),
(95, 0xa901bf000601000000eb1a00),
(96, 0xa301ba000601000000dc0600),
(96, 0xa401ba000601000000dd0600),
(96, 0xa501bf000601000000eb1a00),
(97, 0xb401b3000701000000e00600),
(97, 0xb501b3000701000000e10600),
(97, 0xb401b5000701000000791b00),
(97, 0xb601b7000701000000821b00),
(98, 0xb401b6000601000000791b00),
(98, 0xb501b7000601000000da0600),
(98, 0xb601b5000601000000821b00),
(98, 0xb501b8000601000000db0600),
(99, 0xce01b3000501000000821b00),
(99, 0xcc01b7000501000000791b00),
(99, 0xce01b9000501000000821b00),
(99, 0xd201b2000501000000da0600),
(99, 0xd201b3000501000000db0600),
(100, 0xce019b000601000000821b00),
(100, 0xd10199000601000000da0600),
(100, 0xd1019a000601000000db0600),
(101, 0x6c0089010701000000e00600),
(101, 0x6d0089010701000000e10600),
(101, 0x6c008c010701000000e00600),
(101, 0x6d008c010701000000e10600),
(101, 0x73008b010701000000ba0400),
(102, 0x720089010601000000da0600),
(102, 0x72008a010601000000db0600),
(102, 0x71008d010601000000bd0400),
(102, 0x73008f010701000000ba0400),
(103, 0x7f00950107010000006d1800),
(103, 0x810091010701000000de0600),
(103, 0x810092010701000000df0600),
(104, 0x7a008e0107010000006a1800),
(104, 0x7d008d010701000000620600),
(104, 0x81008b010701000000e00600),
(104, 0x82008b010701000000e10600),
(104, 0x80008d0107010000006a1800),
(104, 0x81008f010701000000e00600),
(104, 0x82008f010701000000e10600),
(105, 0x7d007e0107010000006d1800),
(105, 0x7d00840106010000006d1800),
(105, 0x7d00840107010000006d1800),
(105, 0x810080010501000000e00600),
(105, 0x810083010501000000e00600),
(105, 0x820080010501000000e10600),
(105, 0x820083010501000000e10600),
(105, 0x830083010501000000e00600),
(105, 0x8000820106010000006a1800),
(105, 0x8000810107010000006f1800),
(105, 0x840083010501000000e10600),
(105, 0x8000870106010000006a1800),
(105, 0x830085010601000000e00600),
(105, 0x830086010601000000e00600),
(105, 0x830087010601000000e00600),
(105, 0x8300850107010000006d1800),
(105, 0x840085010601000000e10600),
(105, 0x840086010601000000e10600),
(105, 0x840087010601000000e10600),
(105, 0x830088010601000000e00600),
(105, 0x830089010601000000e00600),
(105, 0x8000880107010000006a1800),
(105, 0x840088010601000000e10600),
(105, 0x840089010601000000e10600),
(106, 0x88007d0107010000006d1800),
(106, 0x890081010701000000e00600),
(106, 0x8a0081010701000000e10600),
(107, 0x860083010701000000de0600),
(107, 0x860084010701000000df0600),
(107, 0x8800850107010000006d1800),
(107, 0x8b00840107010000006a1800),
(107, 0x8d0085010701000000711800),
(107, 0x8e00870107010000006a1800),
(108, 0x9e0094010701000000bc0400),
(108, 0x9c0098010701000000bc0400),
(108, 0x9c009a010701000000dc0600),
(108, 0x9d009a010701000000dd0600),
(109, 0x6a0058010701000000da0600),
(109, 0x6a0059010701000000db0600),
(109, 0x6f0059010701000000691800),
(110, 0x6f00520107010000006d1800),
(110, 0x700050010701000000da0600),
(110, 0x700051010701000000db0600),
(110, 0x730055010701000000691800),
(111, 0xb5007b010601000000e00600),
(111, 0xb6007b010601000000e10600),
(111, 0xba007a0106010000006c1800),
(111, 0xb5007e010601000000e00600),
(111, 0xb6007e010601000000e10600),
(111, 0xb7007d0106010000006a1800),
(112, 0xb70073010601000000da0600),
(112, 0xb600750106010000006c1800),
(112, 0xb70074010601000000db0600),
(112, 0xb800780106010000006a1800);

-- --------------------------------------------------------

--
-- Estrutura da tabela `towns`
--

CREATE TABLE `towns` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `posx` int(11) NOT NULL DEFAULT '0',
  `posy` int(11) NOT NULL DEFAULT '0',
  `posz` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Extraindo dados da tabela `towns`
--

INSERT INTO `towns` (`id`, `name`, `posx`, `posy`, `posz`) VALUES
(1, 'Trekolt', 95, 117, 7),
(2, 'Rhyves', 159, 387, 6),
(3, 'Varak', 242, 429, 12),
(4, 'Jorvik', 496, 172, 7),
(5, 'Saund', 240, 566, 7);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `accounts`
--
ALTER TABLE `accounts`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`);

--
-- Indexes for table `account_bans`
--
ALTER TABLE `account_bans`
  ADD PRIMARY KEY (`account_id`),
  ADD KEY `banned_by` (`banned_by`);

--
-- Indexes for table `account_ban_history`
--
ALTER TABLE `account_ban_history`
  ADD PRIMARY KEY (`id`),
  ADD KEY `account_id` (`account_id`),
  ADD KEY `banned_by` (`banned_by`);

--
-- Indexes for table `account_viplist`
--
ALTER TABLE `account_viplist`
  ADD UNIQUE KEY `account_player_index` (`account_id`,`player_id`),
  ADD KEY `player_id` (`player_id`);

--
-- Indexes for table `guilds`
--
ALTER TABLE `guilds`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`),
  ADD UNIQUE KEY `ownerid` (`ownerid`);

--
-- Indexes for table `guildwar_kills`
--
ALTER TABLE `guildwar_kills`
  ADD PRIMARY KEY (`id`),
  ADD KEY `warid` (`warid`);

--
-- Indexes for table `guild_invites`
--
ALTER TABLE `guild_invites`
  ADD PRIMARY KEY (`player_id`,`guild_id`),
  ADD KEY `guild_id` (`guild_id`);

--
-- Indexes for table `guild_membership`
--
ALTER TABLE `guild_membership`
  ADD PRIMARY KEY (`player_id`),
  ADD KEY `guild_id` (`guild_id`),
  ADD KEY `rank_id` (`rank_id`);

--
-- Indexes for table `guild_ranks`
--
ALTER TABLE `guild_ranks`
  ADD PRIMARY KEY (`id`),
  ADD KEY `guild_id` (`guild_id`);

--
-- Indexes for table `guild_wars`
--
ALTER TABLE `guild_wars`
  ADD PRIMARY KEY (`id`),
  ADD KEY `guild1` (`guild1`),
  ADD KEY `guild2` (`guild2`);

--
-- Indexes for table `houses`
--
ALTER TABLE `houses`
  ADD PRIMARY KEY (`id`),
  ADD KEY `owner` (`owner`),
  ADD KEY `town_id` (`town_id`);

--
-- Indexes for table `house_lists`
--
ALTER TABLE `house_lists`
  ADD KEY `house_id` (`house_id`);

--
-- Indexes for table `ip_bans`
--
ALTER TABLE `ip_bans`
  ADD PRIMARY KEY (`ip`),
  ADD KEY `banned_by` (`banned_by`);

--
-- Indexes for table `market_history`
--
ALTER TABLE `market_history`
  ADD PRIMARY KEY (`id`),
  ADD KEY `player_id` (`player_id`,`sale`);

--
-- Indexes for table `market_offers`
--
ALTER TABLE `market_offers`
  ADD PRIMARY KEY (`id`),
  ADD KEY `sale` (`sale`,`itemtype`),
  ADD KEY `created` (`created`),
  ADD KEY `player_id` (`player_id`);

--
-- Indexes for table `players`
--
ALTER TABLE `players`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`),
  ADD KEY `account_id` (`account_id`),
  ADD KEY `vocation` (`vocation`);

--
-- Indexes for table `players_online`
--
ALTER TABLE `players_online`
  ADD PRIMARY KEY (`player_id`);

--
-- Indexes for table `player_deaths`
--
ALTER TABLE `player_deaths`
  ADD KEY `player_id` (`player_id`),
  ADD KEY `killed_by` (`killed_by`),
  ADD KEY `mostdamage_by` (`mostdamage_by`);

--
-- Indexes for table `player_namelocks`
--
ALTER TABLE `player_namelocks`
  ADD PRIMARY KEY (`player_id`),
  ADD KEY `namelocked_by` (`namelocked_by`);

--
-- Indexes for table `server_config`
--
ALTER TABLE `server_config`
  ADD PRIMARY KEY (`config`);

--
-- Indexes for table `store_history`
--
ALTER TABLE `store_history`
  ADD PRIMARY KEY (`id`),
  ADD KEY `account_id` (`account_id`);

--
-- Indexes for table `tile_store`
--
ALTER TABLE `tile_store`
  ADD KEY `house_id` (`house_id`);

--
-- Indexes for table `towns`
--
ALTER TABLE `towns`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `accounts`
--
ALTER TABLE `accounts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `account_ban_history`
--
ALTER TABLE `account_ban_history`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `guilds`
--
ALTER TABLE `guilds`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `guildwar_kills`
--
ALTER TABLE `guildwar_kills`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `guild_ranks`
--
ALTER TABLE `guild_ranks`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `guild_wars`
--
ALTER TABLE `guild_wars`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `houses`
--
ALTER TABLE `houses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=113;

--
-- AUTO_INCREMENT for table `market_history`
--
ALTER TABLE `market_history`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `market_offers`
--
ALTER TABLE `market_offers`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `players`
--
ALTER TABLE `players`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- AUTO_INCREMENT for table `store_history`
--
ALTER TABLE `store_history`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `towns`
--
ALTER TABLE `towns`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;

--
-- Constraints for dumped tables
--

--
-- Limitadores para a tabela `account_bans`
--
ALTER TABLE `account_bans`
  ADD CONSTRAINT `account_bans_ibfk_1` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `account_bans_ibfk_2` FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Limitadores para a tabela `account_ban_history`
--
ALTER TABLE `account_ban_history`
  ADD CONSTRAINT `account_ban_history_ibfk_1` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `account_ban_history_ibfk_2` FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Limitadores para a tabela `account_viplist`
--
ALTER TABLE `account_viplist`
  ADD CONSTRAINT `account_viplist_ibfk_1` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `account_viplist_ibfk_2` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `guilds`
--
ALTER TABLE `guilds`
  ADD CONSTRAINT `guilds_ibfk_1` FOREIGN KEY (`ownerid`) REFERENCES `players` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `guildwar_kills`
--
ALTER TABLE `guildwar_kills`
  ADD CONSTRAINT `guildwar_kills_ibfk_1` FOREIGN KEY (`warid`) REFERENCES `guild_wars` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `guild_invites`
--
ALTER TABLE `guild_invites`
  ADD CONSTRAINT `guild_invites_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `guild_invites_ibfk_2` FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `guild_membership`
--
ALTER TABLE `guild_membership`
  ADD CONSTRAINT `guild_membership_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `guild_membership_ibfk_2` FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `guild_membership_ibfk_3` FOREIGN KEY (`rank_id`) REFERENCES `guild_ranks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Limitadores para a tabela `guild_ranks`
--
ALTER TABLE `guild_ranks`
  ADD CONSTRAINT `guild_ranks_ibfk_1` FOREIGN KEY (`guild_id`) REFERENCES `guilds` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `house_lists`
--
ALTER TABLE `house_lists`
  ADD CONSTRAINT `house_lists_ibfk_1` FOREIGN KEY (`house_id`) REFERENCES `houses` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `ip_bans`
--
ALTER TABLE `ip_bans`
  ADD CONSTRAINT `ip_bans_ibfk_1` FOREIGN KEY (`banned_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Limitadores para a tabela `market_history`
--
ALTER TABLE `market_history`
  ADD CONSTRAINT `market_history_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `market_offers`
--
ALTER TABLE `market_offers`
  ADD CONSTRAINT `market_offers_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `players`
--
ALTER TABLE `players`
  ADD CONSTRAINT `players_ibfk_1` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `player_deaths`
--
ALTER TABLE `player_deaths`
  ADD CONSTRAINT `player_deaths_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `player_namelocks`
--
ALTER TABLE `player_namelocks`
  ADD CONSTRAINT `player_namelocks_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `player_namelocks_ibfk_2` FOREIGN KEY (`namelocked_by`) REFERENCES `players` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Limitadores para a tabela `store_history`
--
ALTER TABLE `store_history`
  ADD CONSTRAINT `store_history_account_fk` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`id`) ON DELETE CASCADE;

--
-- Limitadores para a tabela `tile_store`
--
ALTER TABLE `tile_store`
  ADD CONSTRAINT `tile_store_ibfk_1` FOREIGN KEY (`house_id`) REFERENCES `houses` (`id`) ON DELETE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
