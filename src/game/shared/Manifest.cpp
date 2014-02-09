#include "Manifest.h"
#include "stlplus\file_system.hpp"
#include "jsoncpp\json.h"
#include <fstream>

#define FLOAT_AT_INDEX(JSON_VALUE, INDEX) float(JSON_VALUE[Json::Value::UInt(INDEX)].asDouble())

bool Manifest::ReadManifest(std::string szFilename)
{
	if (!stlplus::is_full_path(szFilename))
	{
		auto szCurrentDirectory = stlplus::folder_current_full();
		szFilename = stlplus::create_filespec(szCurrentDirectory, szFilename);
	}
	else
	{
		szFilename = stlplus::filespec_to_path(szFilename);
	}

	std::ifstream oManifestStream(szFilename);
	std::string szManifestJson((std::istreambuf_iterator<char>(oManifestStream)),
					 std::istreambuf_iterator<char>());

	Json::Reader oReader;
	bool bParsed = oReader.parse(szManifestJson, m_oManifest);
	if (!bParsed)
	{
		Debug::WarningMessage("Unable to parse %: %",
			szFilename, oReader.getFormatedErrorMessages());
	}

	m_szPath = szFilename;
	return bParsed;
}

void Manifest::WriteManifest(std::string szFilename)
{
	if (szFilename.size() > 0)
	{
		if (!stlplus::is_full_path(szFilename))
		{
			auto szCurrentDirectory = stlplus::folder_current_full();
			szFilename = stlplus::create_filespec(szCurrentDirectory, szFilename);
		}
		else
		{
			szFilename = stlplus::filespec_to_path(szFilename);
		}
	}
	else
	{
		szFilename = m_szPath;
	}

	Json::StyledWriter oWriter;
	std::string szManifest = oWriter.write(m_oManifest);
	std::ofstream oManifestStream(szFilename);
	oManifestStream << szManifest;
	oManifestStream.close();
}

bool Manifest::GetBool(std::string szKey, bool bDefault)
{
	return m_oManifest.get(szKey, bDefault).asBool();
}

void Manifest::SetBool(std::string szKey, bool bValue)
{
	m_oManifest[szKey] = bValue;
}

int Manifest::GetInt(std::string szKey, int iDefault)
{
	return m_oManifest.get(szKey, iDefault).asInt();
}

void Manifest::SetInt(std::string szKey, int iValue)
{
	m_oManifest[szKey] = iValue;
}

float Manifest::GetFloat(std::string szKey, float flDefault)
{
	return float(m_oManifest.get(szKey, flDefault).asDouble());
}

void Manifest::SetFloat(std::string szKey, float flValue)
{
	m_oManifest[szKey] = flValue;
}

std::string Manifest::GetString(std::string szKey, std::string szDefault)
{
	return m_oManifest.get(szKey, szDefault).asString();
}

void Manifest::SetString(std::string szKey, std::string szValue)
{
	m_oManifest[szKey] = szValue;
}

std::string Manifest::GetFolder(std::string szKey, std::string szDefault)
{
	std::string szPath = m_oManifest.get(szKey, "").asString();
	if (szPath.length() == 0)
	{
		return szDefault;
	}
	else
	{
		return stlplus::folder_to_path(stlplus::folder_part(m_szPath), szPath);
	}
}

std::string Manifest::GetFile(std::string szKey, std::string szDefault)
{
	std::string szPath = m_oManifest.get(szKey, std::string()).asString();
	if (szPath.length() == 0)
	{
		return szDefault;
	}
	else
	{
		return stlplus::filespec_to_path(stlplus::folder_part(m_szPath), szPath);
	}
}

Vector Manifest::GetVector(std::string szKey, Vector veDefault)
{
	Json::Value oJsonValue = m_oManifest[szKey];
	if (oJsonValue.size() != 3)
		return veDefault;

	return Vector(FLOAT_AT_INDEX(oJsonValue, 0), FLOAT_AT_INDEX(oJsonValue, 1), FLOAT_AT_INDEX(oJsonValue, 2));
}

void Manifest::SetVector(std::string szKey, Vector veValue)
{
	Json::Value oValue;
	oValue.append(veValue.x);
	oValue.append(veValue.y);
	oValue.append(veValue.z);
	m_oManifest[szKey] = oValue;
}

std::vector<Point> Manifest::GetPointList(std::string szKey)
{
	std::vector<Point> oPoints;
	auto oJsonValue = m_oManifest[szKey];
	for (Json::Value::UInt i = 0; i < oJsonValue.size(); i++)
	{
		auto oPoint = oJsonValue[i];
		oPoints.push_back(Point(FLOAT_AT_INDEX(oPoint, 0), FLOAT_AT_INDEX(oPoint, 1)));
	}

	return oPoints;
}

Point Manifest::GetPoint(std::string szKey, Point poDefault)
{
	Json::Value oJsonValue = m_oManifest[szKey];
	return Point(FLOAT_AT_INDEX(oJsonValue, 0), FLOAT_AT_INDEX(oJsonValue, 1));
}

void Manifest::SetPoint(std::string szKey, Point poValue)
{
	Json::Value oValue;
	oValue.append(poValue.x);
	oValue.append(poValue.y);
	m_oManifest[szKey] = oValue;
}

Manifest Manifest::GetManifest(std::string szKey)
{
	return Manifest(m_oManifest[szKey]);
}

void Manifest::SetManifest(std::string szKey, Manifest oManifest)
{
	m_oManifest[szKey] = oManifest.m_oManifest;
}

Manifest Manifest::GetIncludedManifest(std::string szKey, std::string szPath)
{
	auto szManifestFilename = GetString(szKey);

	if (szPath.length() == 0)
		szPath = stlplus::folder_part(m_szPath);

	szPath = stlplus::create_filespec(szPath, szManifestFilename);

	Manifest oManifestReader;
	oManifestReader.ReadManifest(szPath);
	return oManifestReader;
}

std::vector<Manifest> Manifest::GetManifestList(std::string szKey)
{
	std::vector<Manifest> oChildManifests;
	Json::Value oJsonValue = m_oManifest[szKey];

	if (!oJsonValue.isArray())
		return oChildManifests;

	for (Json::Value::UInt i = 0; i < oJsonValue.size(); i++)
	{
		Json::Value oJsonSubValue = oJsonValue[i];
		if (!oJsonSubValue.isObject())
			continue;

		Manifest oChildManifest(oJsonSubValue);
		oChildManifest.m_szPath = m_szPath;
		oChildManifests.push_back(oChildManifest);
	}

	return oChildManifests;
}

void Manifest::SetManifestList(std::string szKey, std::vector<Manifest> oManifestList)
{
	Json::Value oJsonValue;
	for (Manifest oManifest : oManifestList)
	{
		oJsonValue.append(oManifest.m_oManifest);
	}

	m_oManifest[szKey] = oJsonValue;
}