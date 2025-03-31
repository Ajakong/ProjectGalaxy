#include "ObjectTag.h"
#include<map>

namespace
{
	const std::map<ObjectTag, std::string> kString =
	{
		{ObjectTag::Stage,"Stage"},
		{ObjectTag::Player,"Player"},
		{ObjectTag::Enemy,"Enemy"},
		{ObjectTag::Takobo,"Takobo"},
		{ObjectTag::Kuribo,"Kuribo"},
		{ObjectTag::Gorori,"Gorori"},
		{ObjectTag::KillerTheSeeker,"KillerTheSeeker"},
		{ObjectTag::SpaceEmperor,"SpaceEmperor"},
		{ObjectTag::EnemyAttack,"EnemyAttack"},
		{ObjectTag::PlayerBullet,"PlayerBullet"},
		{ObjectTag::EnemyBullet,"EnemyBullet"},
		{ObjectTag::WarpGate,"WarpGate"},
		{ObjectTag::FullPowerDropItem,"FullPowerDropItem"},
		{ObjectTag::StickStarItem,"StickStarItem"},
		{ObjectTag::Coin,"Coin"},
		{ObjectTag::ClearObject,"ClearObject"},
		{ObjectTag::StarCapture,"StarCapture"},
		{ObjectTag::SeekerLine,"SeekerLine"},
		{ObjectTag::Crystal,"Crystal"},
		{ObjectTag::InvisibleObject,"InvisibleObject"},
		{ObjectTag::End,"End"},
	};

}

std::string ObjectTag_String(ObjectTag tag)
{
	//ここで止まったら定数内にオブジェクトが定義されていない
	return kString.at(tag);
}
